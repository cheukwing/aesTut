#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "key.h"

void addRoundKey(byte_t *state, byte_t *expKey) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    state[i] ^= expKey[i];
  }
}

void encryptByteSub(byte_t *state) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    state[i] = encryptByte(state[i]);
  }
}

void decryptByteSub(byte_t *state) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    state[i] = decryptByte(state[i]);
  }
}

void encryptShiftRow(byte_t *state) {
  arrangeMatrix(state);
  for (int i = 0; i < ROW_LENGTH; ++i) {
    shiftRight(state, i);
    state += ROW_LENGTH;
  }
}

void decryptShiftRow(byte_t *state) {
  for (int i = 0; i < ROW_LENGTH; ++i) {
    shiftLeft(state, i);
    state += ROW_LENGTH;
  }
  arrangeMatrix(state - (ROW_LENGTH * ROW_LENGTH));
}

// generic mix column function
void mixColumn(byte_t *state, byte_t *matrix) {
  byte_t *temp = malloc(sizeof(byte_t) * BLOCK_SIZE);
  memcpy(temp, state, sizeof(byte_t) * BLOCK_SIZE);

  // iterate through row
  for (int i = 0; i < ROW_LENGTH; ++i) {
    // iterate through columns
    for (int j = 0; j < ROW_LENGTH; ++j) {
      // perform the mix column
      byte_t res = 0;
      for (int k = 0; k < ROW_LENGTH; ++k) {
        res ^= galoisMultiply(temp[k * ROW_LENGTH + j], matrix[i * ROW_LENGTH + k]);
      }
      state[i * ROW_LENGTH + j] = res;
    }
  }

  free(temp);
}

void encryptMixColumn(byte_t *state) {
  mixColumn(state, mixColMatrix);
}

void decryptMixColumn(byte_t *state) {
  mixColumn(state, invMixColMatrix);
}

void encrypt(byte_t *state, byte_t *expKey) {
  addRoundKey(state, expKey);
  for (int i = 0; i < 9; i++) {
    expKey += KEY_SIZE;
    encryptByteSub(state);
    encryptShiftRow(state);
    encryptMixColumn(state);
    addRoundKey(state, expKey);
  }
  expKey += KEY_SIZE;
  encryptByteSub(state);
  encryptShiftRow(state);
  addRoundKey(state, expKey);
}

void decrypt(byte_t *state, byte_t *expKey) {
  expKey += 10 * KEY_SIZE;
  addRoundKey(state, expKey);
  for (int i = 9; i > 0; i--) {
    expKey -= KEY_SIZE;
    decryptShiftRow(state);
    decryptByteSub(state);
    addRoundKey(state, expKey);
    decryptMixColumn(state);
  }
  expKey -= KEY_SIZE;
  decryptShiftRow(state);
  decryptByteSub(state);
  addRoundKey(state, expKey);
}

int main() {
  byte_t key[] = "This Is A Secret";
  byte_t state[] = "Hello Everybody!";

  printf("The original message is: ");
  printBytes(state, 16);
  printf("This will be decrypted using the key: ");
  printBytes(key, 16);

  printf("\n");

  printf("Expanding key...\n");
  byte_t *expKey = getExpandedKey(key);
  printf("Expanded key: ");
  printBytes(expKey, 176);

  printf("\n");

  printf("Encrypting...\n");
  encrypt(state, expKey);
  printf("The encrypted message is: ");
  printBytes(state, 16);

  printf("\n");

  printf("Decrypting...\n");
  decrypt(state, expKey);
  printf("The decrypted message is: ");
  printBytes(state, 16);

  free(expKey);

  return EXIT_SUCCESS;
}

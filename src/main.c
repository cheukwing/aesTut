#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "key.h"

void addRoundKey(byte_t **state, byte_t *expKey) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    (*state)[i] ^= expKey[i];
  }
}

void encryptByteSub(byte_t **state) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    (*state)[i] = encryptByte((*state)[i]);
  }
}

void decryptByteSub(byte_t **state) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    (*state)[i] = decryptByte((*state)[i]);
  }
}

void encryptShiftRow(byte_t **state) {
  arrangeMatrix(state);
  for (int i = 0; i < ROW_LENGTH; ++i) {
    shiftRight(state + (ROW_LENGTH * i), i);
  }
}

void decryptShiftRow(byte_t **state) {
  for (int i = 0; i < ROW_LENGTH; ++i) {
    shiftLeft(state + (ROW_LENGTH + i), i);
  }
  arrangeMatrix(state);
}

// generic function
void mixColumn(byte_t **state, byte_t *matrix) {
  byte_t *temp = malloc(sizeof(byte_t) * BLOCK_SIZE);
  memcpy(temp, state, sizeof(temp));

  // iterate through row
  for (int i = 0; i < ROW_LENGTH; ++i) {
    // iterate through columns
    for (int j = 0; j < ROW_LENGTH; ++j) {
      // perform the mix column
      byte_t res = 0;
      for (int k = 0; k < ROW_LENGTH; ++k) {
        res ^= galoisMultiply(temp[k * ROW_LENGTH + j], matrix[i * ROW_LENGTH + k]);
      }
      (*state)[i * ROW_LENGTH + j] = res;
    }
  }

  free(temp);
}

void encryptMixColumn(byte_t **state) {
  mixColumn(state, mixColMatrix);
}

void decryptMixColumn(byte_t **state) {
  mixColumn(state, invMixColMatrix);
}

void encrypt(byte_t **state, byte_t *expKey) {
  addRoundKey(state, expKey);
  for (int i = 0; i < 9; i++) {
    encryptByteSub(state);
    encryptShiftRow(state);
    encryptMixColumn(state);
    addRoundKey(state, expKey + ((i + 1) * KEY_SIZE));
  }
  encryptByteSub(state);
  encryptShiftRow(state);
  addRoundKey(state, expKey + (10 * KEY_SIZE));
}

void decrypt(byte_t **state, byte_t *expKey) {
  addRoundKey(state, expKey + (10 * KEY_SIZE));
  for (int i = 9; i > 0; i--) {
    decryptShiftRow(state);
    decryptByteSub(state);
    addRoundKey(state, expKey + (i * KEY_SIZE));
    decryptMixColumn(state);
  }
  decryptShiftRow(state);
  decryptByteSub(state);
  addRoundKey(state, expKey);
}


int main() {
  byte_t key[17] = "abcabcabcabcabc\0";
  byte_t state[17] = "Hello everybody\0";
  byte_t *stateP = state;

  printf("Expanding key...\n");
  byte_t *expKey = getExpandedKey(key);
  printf("Encrypting...\n");
  encrypt(&stateP, expKey);
  printBytes(stateP, 16);
}
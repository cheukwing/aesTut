#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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
}

void encryptMixColumn(byte_t **state) {
  mixColumn(state, mixColMatrix);
}

void decryptMixColumn(byte_t **state) {
  mixColumn(state, invMixColMatrix);
}


int main() {
  return 0;
}
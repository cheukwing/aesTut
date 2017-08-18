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

void encryptMixColumn(byte_t **state) {
  byte_t *temp = malloc(sizeof(byte_t) * BLOCK_SIZE);
  memcpy(temp, state, sizeof(temp));

  for (int i = 0; i < BLOCK_SIZE; ++i) {
    
  }
}


int main() {
  return 0;
}
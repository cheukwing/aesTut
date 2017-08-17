#include <stdio.h>
#include <stdlib.h>
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

int main() {
  return 0;
}
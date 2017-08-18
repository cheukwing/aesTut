#include <stdlib.h>

#ifndef AESTUT_UTILS_H
#define AESTUT_UTILS_H

#define BLOCK_SIZE 16
#define ROW_LENGTH 4

typedef __uint8_t byte_t;

typedef struct coord {
  int x;
  int y;
} coord_t;

extern byte_t sBox[256];
extern byte_t invSBox[256];

byte_t encryptByte(byte_t);

byte_t decryptByte(byte_t);

void shiftRight(byte_t **, int);

void shiftLeft(byte_t **, int);

void arrangeMatrix(byte_t **);

byte_t galoisMultiply(byte_t, byte_t);

#endif //AESTUT_UTILS_H

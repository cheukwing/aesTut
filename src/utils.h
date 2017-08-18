#include <stdlib.h>

#ifndef AESTUT_UTILS_H
#define AESTUT_UTILS_H

#define BYTES_IN_WORD 4
#define BITS_IN_BYTE 8
#define BLOCK_SIZE 16
#define ROW_LENGTH 4
#define KEY_SIZE 16

typedef __uint8_t byte_t;
typedef __uint32_t word_t;

typedef struct coord {
  int x;
  int y;
} coord_t;

extern byte_t sBox[256];
extern byte_t invSBox[256];
extern byte_t mixColMatrix[16];
extern byte_t invMixColMatrix[16];

byte_t getByteFromWord(word_t, int);

byte_t encryptByte(byte_t);

byte_t decryptByte(byte_t);

void shiftRight(byte_t **, int);

void shiftLeft(byte_t **, int);

void arrangeMatrix(byte_t **);

byte_t galoisMultiply(byte_t, byte_t);

#endif //AESTUT_UTILS_H

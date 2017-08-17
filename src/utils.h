#ifndef AESTUT_UTILS_H
#define AESTUT_UTILS_H

#define BLOCK_SIZE 16

typedef __uint8_t byte_t;

extern byte_t sBox[256];
extern byte_t invSBox[256];

byte_t encryptByte(byte_t);

byte_t decryptByte(byte_t);

#endif //AESTUT_UTILS_H

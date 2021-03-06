#include <stdio.h>
#include "key.h"

#define EXPANDED_KEY_LENGTH 176
#define EXPANSION_ROUNDS 44

word_t rotWord(word_t word) {
  byte_t fstByte = getByteFromWord(word, 0);
  word >>= BITS_IN_BYTE;
  word += (fstByte << (BITS_IN_BYTE * (BYTES_IN_WORD - 1)));
  return word;
}

word_t subWord(word_t word) {
  word_t res = 0;
  for (int i = 0; i < BYTES_IN_WORD; ++i) {
    byte_t encryptedByte = encryptByte(getByteFromWord(word, i));
    res += (encryptedByte << (BITS_IN_BYTE * i));
  }
  return res;
}

// assume keysize always 16
word_t rcon(int round) {
  switch (round) {
    case 0: return 0x01000000;
    case 1: return 0x02000000;
    case 2: return 0x04000000;
    case 3: return 0x08000000;
    case 4: return 0x10000000;
    case 5: return 0x20000000;
    case 6: return 0x40000000;
    case 7: return 0x80000000;
    case 8: return 0x1B000000;
    case 9: return 0x36000000;
    case 10: return 0x6C000000;
    case 11: return 0xD8000000;
    case 12: return 0xAB000000;
    case 13: return 0x4D000000;
    case 14: return 0x9A000000;
    default:
      printf("%d\n", round);
      perror("switch error in rcon");
      exit(EXIT_FAILURE);
  }
}

word_t wordFromOffset(int offset, byte_t *bytes) {
  word_t res = 0;
  for (int i = 0; i < BYTES_IN_WORD; ++i) {
    res += bytes[offset + i] << (BITS_IN_BYTE * i);
  }
  return res;
}

word_t ek(int offset, byte_t *eKey) {
  return wordFromOffset(offset, eKey);
}

word_t k(int offset, byte_t *key) {
  return wordFromOffset(offset, key);
}

byte_t *getExpandedKey(byte_t *key) {
  byte_t *expandedKey = malloc(sizeof(byte_t) * EXPANDED_KEY_LENGTH);
  if (expandedKey == NULL) {
    perror("malloc error in getExpandedKey");
    exit(EXIT_FAILURE);
  }
  byte_t *offset = expandedKey;
  for (int round = 0; round < EXPANSION_ROUNDS; ++round) {
    if (round < 4) {
      addWordToByteArray(offset, k(round * BYTES_IN_WORD, key));
    } else if (round % 4 == 0) {
      addWordToByteArray(offset,
                         subWord(rotWord(ek((round - 1) * 4, expandedKey)))
                             ^ rcon((round / 4) - 1)
                             ^ ek((round - 4) * 4, expandedKey));
    } else {
      addWordToByteArray(offset,
                         ek((round - 1) * 4, expandedKey)
                         ^ ek((round - 4) * 4, expandedKey));
    }
    offset += BYTES_IN_WORD;
  }
  return expandedKey;
}

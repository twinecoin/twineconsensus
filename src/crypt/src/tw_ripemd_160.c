/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <assert.h>
#include <string.h>

#include "tw_ripemd_160.h"
#include "tw_hash.h"

#define BLOCK_WORDS 16
#define BLOCK_BYTES (BLOCK_WORDS * 4)

// Functions

#define NOT(x) (~(x))
#define ROTL_32(x, b) (((x) << (b)) | ((x) >> (32 - (b))))

#define F_0_15(x, y, z)  ((x) ^ (y) ^ (z))
#define F_16_31(x, y, z) (((x) & (y)) | (NOT(x) & (z)))
#define F_32_47(x, y, z) (((x) | NOT(y)) ^ (z))
#define F_48_63(x, y, z) (((x) & (z)) | ((y) & NOT(z)))
#define F_64_79(x, y, z) ((x) ^ ((y) | NOT(z)))

// Addition constants
tw_u32 K_0_15       = 0x00000000UL;
tw_u32 K_16_31      = 0x5A827999UL;
tw_u32 K_32_47      = 0x6ED9EBA1UL;
tw_u32 K_48_63      = 0x8F1BBCDCUL;
tw_u32 K_64_79      = 0xA953FD4EUL;

tw_u32 K_d_0_15     = 0x50A28BE6UL;
tw_u32 K_d_16_31    = 0x5C4DD124UL;
tw_u32 K_d_32_47    = 0x6D703EF3UL;
tw_u32 K_d_48_63    = 0x7A6D76E9UL;
tw_u32 K_d_64_79    = 0x00000000UL;

// Word selection
int r_0_15[]        = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int r_16_31[]       = {7,4,13,1,10,6,15,3,12,0,9,5,2,14,11,8};
int r_32_47[]       = {3,10,14,4,9,15,8,1,2,7,0,6,13,11,5,12};
int r_48_63[]       = {1,9,11,10,0,8,12,4,13,3,7,15,14,5,6,2};
int r_64_79[]       = {4,0,5,9,7,12,2,10,14,1,3,8,11,6,15,13};

int r_d_0_15[]      = {5,14,7,0,9,2,11,4,13,6,15,8,1,10,3,12};
int r_d_16_31[]     = {6,11,3,7,0,13,5,10,14,15,8,12,4,9,1,2};
int r_d_32_47[]     = {15,5,1,3,7,14,6,9,11,8,12,2,10,0,4,13};
int r_d_48_63[]     = {8,6,4,1,3,11,15,0,5,12,2,13,9,7,10,14};
int r_d_64_79[]     = {12,15,10,4,1,5,8,7,6,2,13,14,0,3,9,11};

// Rotation constants
int s_0_15[]        = {11,14,15,12,5,8,7,9,11,13,14,15,6,7,9,8};
int s_16_31[]       = {7,6,8,13,11,9,7,15,7,12,15,9,11,7,13,12};
int s_32_47[]       = {11,13,6,7,14,9,13,15,14,8,13,6,5,12,7,5};
int s_48_63[]       = {11,12,14,15,14,15,9,8,9,14,5,6,8,6,5,12};
int s_64_79[]       = {9,15,5,11,6,8,13,12,5,12,13,14,11,8,5,6};

int s_d_0_15[]      = {8,9,9,11,13,15,15,5,7,7,8,11,14,14,12,6};
int s_d_16_31[]     = {9,13,15,7,12,8,9,11,7,7,12,7,6,15,13,11};
int s_d_32_47[]     = {9,7,15,11,8,6,6,14,12,13,5,14,13,13,7,5};
int s_d_48_63[]     = {15,5,8,11,14,14,6,14,6,9,12,9,12,5,15,8};
int s_d_64_79[]     = {8,5,12,9,12,5,14,6,8,13,6,5,15,13,11,11};

tw_u32 tw_le_to_word(const tw_u8* bytes, const int pos) {
  int w = pos << 2;
  tw_u32 word;
  word  = bytes[w];
  word |= bytes[w + 1] << 8;
  word |= bytes[w + 2] << 16;
  word |= bytes[w + 3] << 24;
  return word;
}

void tw_word_to_le(tw_u8* bytes, const int pos, tw_u32 word) {
  bytes[pos]     = word;
  bytes[pos + 1] = word >> 8;
  bytes[pos + 2] = word >> 16;
  bytes[pos + 3] = word >> 24;
}

void tw_ripemd_160_block(tw_u32* h, const tw_u8* block_bytes) {
  tw_u32 block_words[BLOCK_WORDS];

  for (int i = 0; i < 16; i++) {
    block_words[i] = tw_le_to_word(block_bytes, i);
  }

  tw_u32 A = h[0];
  tw_u32 B = h[1];
  tw_u32 C = h[2];
  tw_u32 D = h[3];
  tw_u32 E = h[4];
  tw_u32 A_d = h[0];
  tw_u32 B_d = h[1];
  tw_u32 C_d = h[2];
  tw_u32 D_d = h[3];
  tw_u32 E_d = h[4];

  // 0 - 15
  for (int i = 0; i < 16; i++) {
    tw_u32 temp, T;
    temp = A + F_0_15(B, C, D) + block_words[r_0_15[i]] + K_0_15;
    T = ROTL_32(temp, s_0_15[i]) + E;
    A = E; E = D; D = ROTL_32(C, 10); C = B; B = T;
    temp = A_d + F_64_79(B_d, C_d, D_d) + block_words[r_d_0_15[i]] + K_d_0_15;
    T = ROTL_32(temp, s_d_0_15[i]) + E_d;
    A_d = E_d; E_d = D_d; D_d = ROTL_32(C_d, 10); C_d = B_d; B_d = T;
  }

  // 16 - 31
  for (int i = 0; i < 16; i++) {
    tw_u32 temp, T;
    temp = A + F_16_31(B, C, D) + block_words[r_16_31[i]] + K_16_31;
    T = ROTL_32(temp, s_16_31[i]) + E;
    A = E; E = D; D = ROTL_32(C, 10); C = B; B = T;
    temp = A_d + F_48_63(B_d, C_d, D_d) + block_words[r_d_16_31[i]] + K_d_16_31;
    T = ROTL_32(temp, s_d_16_31[i]) + E_d;
    A_d = E_d; E_d = D_d; D_d = ROTL_32(C_d, 10); C_d = B_d; B_d = T;
  }

  // 32 - 47
  for (int i = 0; i < 16; i++) {
    tw_u32 temp, T;
    temp = A + F_32_47(B, C, D) + block_words[r_32_47[i]] + K_32_47;
    T = ROTL_32(temp, s_32_47[i]) + E;
    A = E; E = D; D = ROTL_32(C, 10); C = B; B = T;
    temp = A_d + F_32_47(B_d, C_d, D_d) + block_words[r_d_32_47[i]] + K_d_32_47;
    T = ROTL_32(temp, s_d_32_47[i]) + E_d;
    A_d = E_d; E_d = D_d; D_d = ROTL_32(C_d, 10); C_d = B_d; B_d = T;
  }

  // 48 - 63
  for (int i = 0; i < 16; i++) {
    tw_u32 temp, T;
    temp = A + F_48_63(B, C, D) + block_words[r_48_63[i]] + K_48_63;
    T = ROTL_32(temp, s_48_63[i]) + E;
    A = E; E = D; D = ROTL_32(C, 10); C = B; B = T;
    temp = A_d + F_16_31(B_d, C_d, D_d) + block_words[r_d_48_63[i]] + K_d_48_63;
    T = ROTL_32(temp, s_d_48_63[i]) + E_d;
    A_d = E_d; E_d = D_d; D_d = ROTL_32(C_d, 10); C_d = B_d; B_d = T;
  }

  // 64 - 79
  for (int i = 0; i < 16; i++) {
    tw_u32 temp, T;
    temp = A + F_64_79(B, C, D) + block_words[r_64_79[i]] + K_64_79;
    T = ROTL_32(temp, s_64_79[i]) + E;
    A = E; E = D; D = ROTL_32(C, 10); C = B; B = T;
    temp = A_d + F_0_15(B_d, C_d, D_d) + block_words[r_d_64_79[i]] + K_d_64_79;
    T = ROTL_32(temp, s_d_64_79[i]) + E_d;
    A_d = E_d; E_d = D_d; D_d = ROTL_32(C_d, 10); C_d = B_d; B_d = T;
  }

  tw_u32 T;
  T =    h[1] + C + D_d;
  h[1] = h[2] + D + E_d;
  h[2] = h[3] + E + A_d;
  h[3] = h[4] + A + B_d;
  h[4] = h[0] + B + C_d;
  h[0] = T;
}

tw_u512 tw_ripemd_160(const tw_u8* message, const tw_u64 len) {
  int full_blocks = len >> 6;

  tw_u32 h[] = {0x67452301UL,
                0xEFCDAB89UL,
                0x98BADCFEUL,
                0x10325476UL,
                0xC3D2E1F0UL};

  for (int block = 0; block < full_blocks; block++) {
    int offset = block << 6;
    tw_ripemd_160_block(h, message + offset);
  }

  int remaining = len & 63;
  int offset = len - remaining;

  tw_u8 padded_bytes[BLOCK_BYTES];
  memcpy(padded_bytes, message + offset, remaining);
  memset(padded_bytes + remaining, 0, BLOCK_BYTES - remaining);

  padded_bytes[remaining] = 0x80;

  if (remaining >= BLOCK_BYTES - 8) {
    tw_ripemd_160_block(h, padded_bytes);
    memset(padded_bytes, 0, BLOCK_BYTES - 8);
  }

  tw_word_to_le(padded_bytes, BLOCK_BYTES - 8, len << 3);
  tw_word_to_le(padded_bytes, BLOCK_BYTES - 4, len >> 29);
  tw_ripemd_160_block(h, padded_bytes);

  tw_u8 digest[64];
  for (int i = 0; i < 20; i += 4) {
    tw_word_to_le(digest, i, h[i >> 2]);
  }

  for (int i = 20; i < 64; i++) {
    digest[i] = 0;
  }

  return tw_le_to_tw_u512(digest);
}

/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <assert.h>
#include "tw_uint.h"

const tw_u512 TW_ZERO        = {0, 0, 0, 0, 0, 0, 0, 0};
const tw_u512 TW_ONE         = {1, 0, 0, 0, 0, 0, 0, 0};
const tw_u512 TW_UINT_MAX    = {TW_U64_MAX, TW_U64_MAX, TW_U64_MAX, TW_U64_MAX,
                                TW_U64_MAX, TW_U64_MAX, TW_U64_MAX, TW_U64_MAX};
const tw_u512 TW_UINT256_MAX = {0,          0,          0,          0,
                                TW_U64_MAX, TW_U64_MAX, TW_U64_MAX, TW_U64_MAX};

void tw_set_512(tw_u512* y, const tw_u512* a) {
  for (int i = 0; i < 8; i++) {
    y->d[i] = a->d[i];
  }
}

void tw_set_64(tw_u512* y, const tw_u64 a) {
  y->d[0] = a;
  for (int i = 1; i < 8; i++) {
    y->d[i] = 0;
  }
}

int tw_equal(const tw_u512* a, const tw_u512* b) {
  for (int i = 0; i < 8; i++) {
    if (a->d[i] != b->d[i]) {
      return 0;
    }
  }
  return 1;
}

int tw_compare(const tw_u512* a, const tw_u512* b) {
  for (int i = 7; i >= 0; i--) {
    tw_u64 aw = a->d[i];
    tw_u64 bw = b->d[i];
    if (aw == bw) {
      continue;
    }
    return (aw > bw) ? 1 : -1;
  }
  return 0;
}

tw_u64 tw_prop_carry_word(tw_u64* y, const tw_u64 a, const tw_u64 carry) {
  *y = a + carry;
  tw_u64 carry_out = a == TW_U64_MAX && carry;
  return carry_out != 0;
}

tw_u64 tw_prop_borrow_word(tw_u64* y, const tw_u64 a, const tw_u64 borrow) {
  *y = a - borrow;
  tw_u64 borrow_out = a == 0 && borrow;
  return borrow_out != 0;
}

tw_u64 tw_add_word(tw_u64* y, const tw_u64 a, const tw_u64 b, const tw_u64 carry) {
  *y = a + b + carry;
  tw_u64 carry_out = (b == TW_U64_MAX && carry) || (*y < a);
  return carry_out != 0;
}

tw_u64 tw_sub_word(tw_u64* y, const tw_u64 a, const tw_u64 b, const tw_u64 borrow) {
  *y = a - b - borrow;
  tw_u64 borrow_out = (b == TW_U64_MAX && borrow) || (*y > a);
  return borrow_out != 0;
}

int tw_add(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 carry = 0;
  for (int i = 0; i < 8; i++) {
    carry = tw_add_word(&y->d[i], a->d[i], b->d[i], carry);
  }
  return carry != 0;
}

int tw_add_32_lshift(tw_u512* y, const tw_u512* a, const tw_u64 b, const tw_u32 left_shift) {
  int masked_shift = left_shift & 15;
  tw_u64 carry = 0;
  int first = masked_shift >> 1;
  int i;
  for (i = 0; i < first; i++) {
    y->d[i] = a->d[i];
  }
  if ((masked_shift & 1) == 0) {
    carry = tw_add_word(&y->d[i], a->d[i], b, carry);
  } else {
    carry = tw_add_word(&y->d[i], a->d[i], b << 32, carry);
    if (i < 7) {
      i++;
      carry = tw_add_word(&y->d[i], a->d[i], (b >> 32) & TW_U32_MAX, carry);
    } else {
      carry = carry || (b >> 32) != 0;
    }
  }
  i++;
  for (;i < 8 && carry != 0; i++) {
    carry = tw_prop_carry_word(&y->d[i], a->d[i], carry);
  }
  for (;i < 8; i++) {
    y->d[i] = a->d[i];
  }
  return carry != 0;
}

int tw_sub(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 borrow = 0;
  for (int i = 0; i < 8; i++) {
    borrow = tw_sub_word(&y->d[i], a->d[i], b->d[i], borrow);
  }
  return borrow != 0;
}

int tw_sub_32_lshift(tw_u512* y, const tw_u512* a, const tw_u64 b, const tw_u32 left_shift) {
  int masked_shift = left_shift & 15;
  tw_u64 borrow = 0;
  int first = masked_shift >> 1;
  int i;
  for (i = 0; i < first; i++) {
    y->d[i] = a->d[i];
  }
  if ((masked_shift & 1) == 0) {
    borrow = tw_sub_word(&y->d[i], a->d[i], b, borrow);
  } else {
    borrow = tw_sub_word(&y->d[i], a->d[i], b << 32, borrow);
    if (i < 7) {
      i++;
      borrow = tw_sub_word(&y->d[i], a->d[i], (b >> 32) & TW_U32_MAX, borrow);
    } else {
      borrow = borrow || (b >> 32) != 0;
    }
  }
  i++;
  for (;i < 8 && borrow != 0; i++) {
    borrow = tw_prop_borrow_word(&y->d[i], a->d[i], borrow);
  }
  for (;i < 8; i++) {
    y->d[i] = a->d[i];
  }
  return borrow != 0;
}

void u512_to_u32_words(tw_u64* y, const tw_u512* a) {
  int j = 0;
  for (int i = 0; i < 8; i++) {
    y[j++] = a->d[i] & TW_U32_MAX;
    y[j++] = a->d[i] >> 32;
  }
}

void u32_words_to_u512(tw_u512* y, const tw_u64* a) {
  int j = 0;
  for (int i = 0; i < 8; i++) {
    y->d[i] = a[j++] & TW_U32_MAX;
    y->d[i] |= a[j++] << 32;
  }
}

int tw_mul_internal(tw_u512* y, tw_u64* a32, tw_u64* b32, int b_len, int b_offset) {
  tw_u64 r[32];
  for (int i = 0; i < 32; i++) {
    r[i] = 0;
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < b_len; j++) {
      tw_u64 product = a32[i] * b32[j];
      r[i + j + b_offset] += product & TW_U32_MAX;
      r[i + j + 1 + b_offset] += product >> 32;
    }
  }

  tw_u64 carry = 0;
  for (int i = 0; i < 32; i++) {
    r[i] += carry;
    carry = r[i] >> 32;
    r[i] &= TW_U32_MAX;
  }

  u32_words_to_u512(y, r);

  for (int i = 16; i < 32; i++) {
    if (r[i] != 0) {
      return 1;
    }
  }
  return 0;
}

int tw_mul(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 a32[16];
  tw_u64 b32[16];
  u512_to_u32_words(a32, a);
  u512_to_u32_words(b32, b);

  return tw_mul_internal(y, a32, b32, 16, 0) != 0;
}

int tw_mul_32_lshift(tw_u512* y, const tw_u512* a, const tw_u64 b, const tw_u32 left_shift) {
  tw_u64 a32[16];
  tw_u64 b32[2];

  u512_to_u32_words(a32, a);
  b32[0] = b & TW_U32_MAX;
  b32[1] = b >> 32;

  int masked_shift = left_shift & 15;

  return tw_mul_internal(y, a32, b32, 2, masked_shift) != 0;
}

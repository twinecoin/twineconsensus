/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <assert.h>
#include "tw_uint.h"

const tw_u512 TW_ZERO        = {0, 0, 0, 0, 0, 0, 0, 0};
const tw_u512 TW_U512_ONE    = {1, 0, 0, 0, 0, 0, 0, 0};
const tw_u512 TW_U512_MAX    = {TW_U64_MAX, TW_U64_MAX, TW_U64_MAX, TW_U64_MAX,
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

tw_u64 tw_add_word(tw_u64* y, const tw_u64 a, const tw_u64 b, const tw_u64 carry) {
  *y = a + b + carry;
  tw_u64 carry_out = (b == TW_U64_MAX && carry) || (*y < a);
  return carry_out != 0;
}

int tw_add(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 carry = 0;
  for (int i = 0; i < 8; i++) {
    carry = tw_add_word(&y->d[i], a->d[i], b->d[i], carry);
  }
  return carry != 0;
}

tw_u64 tw_sub_word(tw_u64* y, const tw_u64 a, const tw_u64 b, const tw_u64 borrow) {
  *y = a - b - borrow;
  tw_u64 borrow_out = (b == TW_U64_MAX && borrow) || (*y > a);
  return borrow_out != 0;
}

int tw_sub(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 borrow = 0;
  for (int i = 0; i < 8; i++) {
    borrow = tw_sub_word(&y->d[i], a->d[i], b->d[i], borrow);
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

int tw_mul_internal(tw_u512* y, const tw_u64* a32, const tw_u64* b32, const int b_len, const int b_offset) {
  int b_offset_masked = b_offset & 15;
  tw_u64 r[33];
  for (int i = 0; i < 33; i++) {
    r[i] = 0;
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < b_len; j++) {
      assert(i + j + 1 + b_offset_masked < 33);
      assert(i + j + b_offset_masked >= 0);
      tw_u64 product = a32[i] * b32[j];
      r[i + j + b_offset_masked] += product & TW_U32_MAX;
      r[i + j + 1 + b_offset_masked] += product >> 32;
    }
  }

  tw_u64 carry = 0;
  for (int i = 0; i < 33; i++) {
    r[i] += carry;
    carry = r[i] >> 32;
    r[i] &= TW_U32_MAX;
  }

  u32_words_to_u512(y, r);

  if (carry) {
    return 1;
  }

  for (int i = 16; i < 33; i++) {
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

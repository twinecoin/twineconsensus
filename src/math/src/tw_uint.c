/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <assert.h>
#include "tw_uint.h"

const tw_u512 TW_U512_ZERO   = {0, 0, 0, 0, 0, 0, 0, 0};
const tw_u512 TW_U512_ONE    = {1, 0, 0, 0, 0, 0, 0, 0};
const tw_u512 TW_U512_MAX    = {TW_U64_MAX, TW_U64_MAX, TW_U64_MAX, TW_U64_MAX,
                                TW_U64_MAX, TW_U64_MAX, TW_U64_MAX, TW_U64_MAX};

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

int tw_add(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 carry = 0;
  for (int i = 0; i < 8; i++) {
    tw_u64 sum = a->d[i] + b->d[i] + carry;
    carry = (b->d[i] == TW_U64_MAX && carry) || (sum < a->d[i]);
    y->d[i] = sum;
  }
  return carry != 0;
}

int tw_sub(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 borrow = 0;
  for (int i = 0; i < 8; i++) {
    tw_u64 diff = a->d[i] - b->d[i] - borrow;
    borrow = (b->d[i] == TW_U64_MAX && borrow) || (diff > a->d[i]);
    y->d[i] = diff;
  }
  return borrow != 0;
}

int tw_lshift(tw_u512* y, const tw_u512*a, const tw_u32 bits) {
  int word_shift = (bits  >> 6) & 0x7;
  int bit_shift = bits & 63;
  int overflow = 0;

  assert(word_shift < 8);

  // These words are shifted fully out of range
  for (int i = 7; i >= 8 - word_shift; i--) {
    if (a->d[i] != 0) {
      overflow = 1;
      break;
    }
  }

  // The top bits of this word overflows if there is a bit shift
  if (bit_shift != 0) {
    overflow |= (a->d[7 - word_shift] >> (64 - bit_shift)) != 0;
  }

  // These words include a portion from two of the input words except only one
  // is required if bit_shift is zero
  for (int i = 7; i >= word_shift; i--) {
    tw_u64 shifted_word = a->d[i - word_shift] << bit_shift;
    if (i > word_shift && bit_shift != 0) {
      shifted_word |= a->d[i - word_shift - 1] >> (64 - bit_shift);
    }
    y->d[i] = shifted_word;
  }

  // All bits in these words are fully shifted left
  for (int i = word_shift - 1; i >= 0; i--) {
    y->d[i] = 0;
  }

  return overflow;
}

int tw_rshift(tw_u512* y, const tw_u512*a, const tw_u32 bits) {
  int word_shift = (bits  >> 6) & 0x7;
  int bit_shift = bits & 63;
  int underflow = 0;

  assert(word_shift < 8);

  // These words are shifted fully out of range
  for (int i = 0; i < word_shift; i++) {
    if (a->d[i] != 0) {
      underflow = 1;
      break;
    }
  }

  // The bottom bits of this word underflows if there is a bit shift
  if (bit_shift != 0) {
    underflow |= (a->d[word_shift] << (64 - bit_shift)) != 0;
  }

  // These words include a portion from two of the input words except only one
  // is required if bit_shift is zero
  for (int i = 0; i < 8 - word_shift; i++) {
    tw_u64 shifted_word = a->d[i + word_shift] >> bit_shift;
    if (i < 7 - word_shift && bit_shift != 0) {
      shifted_word |= a->d[i + word_shift + 1] << (64 - bit_shift);
    }
    y->d[i] = shifted_word;
  }

  // All bits in these words are fully shifted right
  for (int i = 8 - word_shift; i < 8; i++) {
    y->d[i] = 0;
  }

  return underflow;
}

int tw_mul(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 a32[16];
  tw_u64 b32[16];
  for (int i = 0; i < 8; i++) {
    int j = i << 1;
    assert(j < 15);
    a32[j] = a->d[i] & TW_U32_MAX;
    b32[j] = b->d[i] & TW_U32_MAX;
    j++;
    a32[j] = a->d[i] >> 32;
    b32[j] = b->d[i] >> 32;
  }

  tw_u64 product_sum[32];
  for (int i = 0; i < 32; i++) {
    product_sum[i] = 0;
  }

  for (int i = 0; i < 16; i++) {
    if (a32[i] == 0) {
      continue;
    }
    for (int j = 0; j < 16; j++) {
      assert(i + j + 1 < 32);
      assert(i + j >= 0);
      tw_u64 product = a32[i] * b32[j];
      product_sum[i + j] += product & TW_U32_MAX;
      product_sum[i + j + 1] += product >> 32;
    }
  }

  tw_u64 carry = 0;
  for (int i = 0; i < 32; i++) {
    product_sum[i] += carry;
    carry = product_sum[i] >> 32;
    product_sum[i] &= TW_U32_MAX;
  }

  assert(carry == 0);

  for (int i = 0; i < 8; i++) {
    int j = i << 1;
    assert(j < 15);
    y->d[i] = (product_sum[j + 1] << 32) | product_sum[j];
  }

  for (int i = 16; i < 32; i++) {
    if (product_sum[i] != 0) {
      return 1;
    }
  }

  return 0;
}

int tw_div_rem(tw_u512* y, tw_u512* z, const tw_u512* a, const tw_u512* b) {
  if (tw_equal(b, &TW_U512_ZERO)) {
    return 1;
  }

  int max_shift = 63;

  for (int i = 7; b->d[i] == 0 && i > 0; i--) {
    max_shift += 64;
  }

  for (int i = 7; a->d[i] == 0 && i >= 0; i--) {
    max_shift -= 64;
  }

  // remainder = a
  tw_u512 remainder = *a;
  // quotient = 0
  tw_u512 quotient = TW_U512_ZERO;
  for (int i = max_shift; i >= 0 && tw_compare(&remainder, b) >= 0; i--) {
    // estimate = 1 << i;
    // product = b * estimate = b * (1 << i) = b << i
    tw_u512 product;
    int overflow = tw_lshift(&product, b, i);
    // if product > TW_U512_MAX then product > remainder, so disregard estimate
    if (overflow) {
      continue;
    }
    // difference = remainder - product
    tw_u512 difference;
    int borrow = tw_sub(&difference, &remainder, &product);
    // if borrow then product > remainder, so disregard estimate
    if (borrow) {
      continue;
    }
    remainder = difference;
    // estimate = 1 << i
    tw_u512 estimate;
    overflow = tw_lshift(&estimate, &TW_U512_ONE, i);
    assert(overflow == 0);
    // quotient = quotient + estimate
    int carry = tw_add(&quotient, &quotient, &estimate);
    assert(carry == 0);
  }
  *y = quotient;
  *z = remainder;
  return 0;
}

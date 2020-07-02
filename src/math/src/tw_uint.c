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

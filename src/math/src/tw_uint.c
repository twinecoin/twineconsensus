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

int tw_add(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  tw_u64 carry = 0;
  for (int i = 0; i < 8; i++) {
    y->d[i] = a->d[i] + b->d[i] + carry;
    carry = (b->d[i] == TW_U64_MAX && carry) || (y->d[i] < a->d[i]);
  }
  return carry != 0;
}

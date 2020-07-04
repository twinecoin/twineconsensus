/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include "tw_test_harness.h"

#include "tw_hash.h"

static int tw_test_harness_lock = 1;

int tw_th_unlock_test_functions(tw_u64 code, int unlock) {
  if (code != TW_TEST_HARNESS_UNLOCK_CODE) {
    return 0;
  }
  int old_value = tw_test_harness_lock;
  tw_test_harness_lock = !unlock;
  return old_value;
}

int tw_th_equal(const tw_u512* a, const tw_u512* b) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_equal(a, b);
}

int tw_th_compare(const tw_u512* a, const tw_u512* b) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_compare(a, b);
}

int tw_th_add(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_add(y, a, b);
}

int tw_th_sub(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_sub(y, a, b);
}

int tw_th_lshift(tw_u512* y, const tw_u512*a, const tw_u32 bitshift) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_lshift(y, a, bitshift);
}

int tw_th_mul(tw_u512* y, const tw_u512* a, const tw_u512* b) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_mul(y, a, b);
}

int tw_th_div_rem(tw_u512* y, tw_u512* z, const tw_u512* a, const tw_u512* b) {
  if (tw_test_harness_lock) {
    return 0;
  }
  return tw_div_rem(y, z, a, b);
}

// tw_hash functions
tw_u512 tw_th_sha_256(const tw_u8* message, const tw_u64 len) {
  if (tw_test_harness_lock) {
    return TW_U512_ZERO;
  }
  return tw_sha_256(message, len);
}

tw_u512 tw_th_sha_512(const tw_u8* message, const tw_u64 len) {
  if (tw_test_harness_lock) {
    return TW_U512_ZERO;
  }
  return tw_sha_512(message, len);
}

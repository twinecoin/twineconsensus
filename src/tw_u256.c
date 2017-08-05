/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <assert.h>
#include "tw_u256.h"

const tw_u256 TW_ZERO = {0, 0, 0, 0, 0, 0, 0, 0};
const tw_u256 TW_ONE = {0, 0, 0, 0, 0, 0, 0, 1};

int tw_equal(const tw_u256* a, const tw_u256* b) {
  for (int i = 7; i >= 0; i--) {
    if (a->d[i] != b->d[i]) {
      return 0;
    }
  }
  return 1;
}

int tw_compare(const tw_u256* a, const tw_u256* b) {
  for (int i = 0; i < 8; i++) {
    tw_u32 aw = a->d[i];
    tw_u32 bw = b->d[i];
    if (aw > bw) {
      return 1;
    }
    if (aw < bw) {
      return -1;
    }
  }
  return 0;
}

int tw_add(tw_u256* y, const tw_u256* a, const tw_u256* b) {
  tw_u64 carry = 0;

  for (int i = 7; i >= 0; i--) {
    tw_u64 result = (tw_u64) a->d[i] + (tw_u64) b->d[i] + carry;
    carry = result >> 32;
    y->d[i] = (tw_u32) result;
  }

  assert(carry == 0 || carry == 1);

  return carry != 0;
}

int tw_add_32(tw_u256* y, const tw_u256* a, const tw_u32 b, const tw_u32 left_shift) {
  tw_set_256(y, a);

  tw_u64 carry = 0;

  int i = 7 - (left_shift & 7);

  tw_u64 result = (tw_u64) a->d[i] + (tw_u64) b + carry;
  carry = result >> 32;
  y->d[i] = (tw_u32) result;

  i--;

  for (; i >= 0 && carry != 0; i--) {
    result = (tw_u64) a->d[i] + carry;
    carry = result >> 32;
    y->d[i] = (tw_u32) result;
  }

  assert(carry == 0 || carry == 1);

  return carry != 0;
}

int tw_sub(tw_u256* y, const tw_u256* a, const tw_u256* b) {
  tw_i64 borrow = 0;

  for (int i = 7; i >= 0; i--) {
    tw_i64 result = (tw_i64) a->d[i] - (tw_i64) b->d[i] + borrow;
    borrow = result >> 32;
    y->d[i] = (tw_u32) result;
  }

  assert(borrow == 0 || borrow == -1);

  return borrow != 0;
}

int tw_sub_32(tw_u256* y, const tw_u256* a, const tw_u32 b, const tw_u32 left_shift) {
  tw_set_256(y, a);

  tw_i64 borrow = 0;

  int i = 7 - (left_shift & 7);

  tw_i64 result = (tw_i64) a->d[i] - (tw_i64) b + borrow;
  borrow = result >> 32;
  y->d[i] = (tw_u32) result;

  i--;

  for (; i >= 0 && borrow != 0; i--) {
    result = (tw_i64) a->d[i] + borrow;
    borrow = result >> 32;
    y->d[i] = (tw_u32) result;
  }

  assert(borrow == 0 || borrow == -1);

  return borrow != 0;
}

int tw_mul_32(tw_u256* y, const tw_u256* a, const tw_u32 b, const tw_u32 left_shift) {
  // Unused words are zero by default
  tw_set_32(y, 0);

  // If b is zero, then use zeroed y
  if (b == 0) {
    return 0;
  }

  int yi = 7 - (left_shift & 7);
  int ai = 7;

  // y1 is write index and is shifted
  tw_u64 carry = 0;
  for (; yi >= 0; yi--) {
    // (u32 * u32) + u32 is guaranteed to be less than max of u64
    tw_u64 mul = ((tw_u64) a->d[ai] * (tw_u64) b) + carry;
    y->d[yi] = (tw_u32) mul;
    carry = mul >> 32;
    ai--;
  }

  // If carry is non-zero, then an overflow happened
  if (carry != 0) {
    return 1;
  }

  // The ai variable indexes bytes in the input that weren't processed.
  // If any are non-zero, then they would cause an overflow, since b must be
  // non-zero by this point.
  while (ai >= 0) {
    if (a->d[ai] != 0) {
      return 1;
    }
    ai--;
  }
  return 0;
}

int tw_mul(tw_u256* y, const tw_u256* a, const tw_u256* b) {
 tw_u256 acc = TW_ZERO;
 tw_u256 mul32;
 int overflow = 0;

 for (int i = 0; i < 8; i++) {
   overflow |= tw_mul_32(&mul32, a, b->d[i], 7 - i);
   overflow |= tw_add(&acc, &acc, &mul32);
 }

 tw_set_256(y, &acc);

 assert(overflow == 0 || overflow == 1);

 return overflow;
}

int tw_div_rem(tw_u256* y, tw_u256* z, const tw_u256* a, const tw_u256* b) {
  // Divide by zero
  if (tw_equal(b, &TW_ZERO)) {
    return 1;
  }

  // Numerator is less than denominator
  if (tw_compare(a, b) < 0) {
    *y = TW_ZERO;
    *z = *a;
    return 0;
  }

  // Special case for when the denominator is near maximum (2 most significant words
  // are maximum).  In this case, the exponent for the test divisor would go out
  // of range when correcting for the overflow of its mantissa.
  if (b->d[0] == TW_U32_MAX && b->d[1] == TW_U32_MAX) {
    assert (tw_compare(a, b) >= 0);
    *y = TW_ONE;
    int borrow = tw_sub(z, a, b);
    assert(borrow == 0);
    return 0;
  }

  // Initialize remainder and divide accumulator
  tw_u256 rem = *a;
  tw_u256 div = TW_ZERO;

  // Scan for MSW of b
  int b_msw_index = 0;
  while (b->d[b_msw_index] == 0 && b_msw_index < 7) {
    b_msw_index++;
  }

  // The test divisor consists of the top 2 MSWs of the divisor
  // and an exponent
  int test_divisor_exp;
  tw_u64 test_divisor_man;

  if (b_msw_index < 7) {
    test_divisor_exp = 6 - b_msw_index;
    test_divisor_man = (((tw_u64) b->d[b_msw_index]) << 32) | ((tw_u64) b->d[b_msw_index + 1]);
  } else {
    test_divisor_exp = 0;
    test_divisor_man = (tw_u64) b->d[b_msw_index];
  }

  // In order to prevent underflow the test divisor must be greater or equal to the
  // actual divisor.

  // No change required if exponent is zero, since test_divisor is equal to divisor
  if (test_divisor_exp != 0) {
    if (test_divisor_man == TW_U64_MAX) {
      // If the test_divisor_man is at full scale, incrementing it would cause an overflow
      // It is right shifted by 32 and the exponent is increased by one.
      test_divisor_man >>= 32;
      test_divisor_exp++;
    }

    // Increment test_divisor_man to ensure the test_divisor is greater
    // than the actual divisor
    test_divisor_man++;
  }

  assert(test_divisor_man != 0);
  assert(test_divisor_exp < 7);

  #ifndef NDEBUG
  int loops = 0;
  #endif

  // Scan for initial MSW of remainder
  int rem_msw_index = 0;
  while (rem.d[rem_msw_index] == 0 && rem_msw_index < 6) {
    rem_msw_index++;
  }

  // Loop until the remainder is less than the divisor
  while (tw_compare(&rem, b) >= 0 && rem_msw_index < 7) {
    #ifndef NDEBUG
    loops++;
    assert(loops <= 16);
    #endif

    // Compute a test remainder for use with the test divisor
    tw_u64 test_rem_man = (((tw_u64) rem.d[rem_msw_index]) << 32) | rem.d[rem_msw_index + 1];
    int rem_exp = 6 - rem_msw_index;
    assert(rem_exp >= 0);

    // Increment the MSW index as the remainder gets smaller, but not
    // past the 2nd LSW.
    if (rem_msw_index < 6 && test_rem_man <= TW_U32_MAX) {
      rem_msw_index++;
      continue;
    }

    // Calculate an estimate of the result of the division
    tw_u64 estimate;
    int shift = rem_exp - test_divisor_exp;

    // The mantissa for the divisor can be greater than for the remainder in two cases.
    // -
    // - The remainder is actually larger but has a lower mantissa
    if (test_divisor_man > test_rem_man) {
      if (rem_exp == test_divisor_exp) {
        // Case 1:  Incrementing the test divisor caused it to be larger than the test
        //          remainder.  Otherwise, they would have been equal
        assert(test_divisor_man == test_rem_man + 1);
        estimate = 1;
        assert(shift == 0);
      } else {
        // Case 2: The remainder has a higher exponent.
        //         A reduced divisor is used in this case and the shift is adjusted.
        tw_u64 reduced_divisor = test_divisor_man >> 32;
        assert(reduced_divisor != TW_U64_MAX);
        estimate = test_rem_man / (reduced_divisor + 1);
        shift--;
      }
    } else {
      estimate = test_rem_man / test_divisor_man;
    }

    assert(shift >= 0);
    assert(estimate != 0);

    tw_u256 sub;

    int overflow = 0;
    int borrow = 0;
    int carry = 0;

    assert(shift < 7);

    // This computes
    //    remainder = remainder - (estimate * divisor)
    //    divisor = divisor + estimate
    //
    // It is critical that the estimate is lower than the correct value to avoid
    // underflows.

    // If the estimate cannot be represented in 32-bits, two passes are required
    if (estimate > 0xFFFFFFFFL) {
       overflow = tw_mul_32(&sub, b, (tw_u32) (estimate >> 32), shift + 1);
       borrow = tw_sub(&rem, &rem, &sub);
       carry = tw_add_32(&div, &div, (tw_u32) (estimate >> 32), shift + 1);
    }

    overflow |= tw_mul_32(&sub, b, estimate, shift);
    borrow |= tw_sub(&rem, &rem, &sub);
    carry |= tw_add_32(&div, &div, estimate, shift);

    assert(estimate != 0);

    assert(!overflow);
    assert(!borrow);
    assert(!carry);
  }

  *y = div;
  *z = rem;

  return 0;
}

void tw_set_256(tw_u256* y, const tw_u256* a) {
  for (int i = 0; i < 8; i++) {
    y->d[i] = a->d[i];
  }
}

void tw_set_32(tw_u256* y, const tw_u32 a) {
  for (int i = 0; i < 7; i++) {
    y->d[i] = 0;
  }
  y->d[7] = a;
}

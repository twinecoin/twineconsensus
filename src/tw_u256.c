#include <assert.h>
#include "tw_u256.h"

int tw_equal(tw_u256* a, tw_u256* b) {
  for (int i = 7; i >= 0; i--) {
    if (a->d[i] != b->d[i]) {
      return 0;
    }
  }
  return 1;
}

int tw_add(tw_u256* y, tw_u256* a, tw_u256* b) {
  tw_u64 carry = 0;

  for (int i = 7; i >= 0; i--) {
    tw_u64 result = (tw_u64) a->d[i] + (tw_u64) b->d[i] + carry;
    carry = result >> 32;
    y->d[i] = (tw_u32) result;
  }

  assert(carry == 0 || carry == 1);

  return carry != 0;
}

int tw_sub(tw_u256* y, tw_u256* a, tw_u256* b) {
  tw_i64 borrow = 0;

  for (int i = 7; i >= 0; i--) {
    tw_i64 result = (tw_i64) a->d[i] - (tw_i64) b->d[i] + borrow;
    borrow = result >> 32;
    y->d[i] = (tw_u32) result;
  }

  assert(borrow == 0 || borrow == -1);

  return borrow != 0;
}

int tw_mul_32(tw_u256* y, tw_u256* a, tw_u32 b, tw_u32 left_shift) {
  // Unused words are zero by default
  tw_set_32(y, 0);

  // If b is zero, then use zeroed y
  if (b == 0) {
    return 0;
  }

  left_shift &= 7;
  int yi = 7 - left_shift;
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

int tw_mul(tw_u256* y, tw_u256* a, tw_u256* b) {
 tw_u256 acc = {0, 0, 0, 0, 0, 0, 0, 0};
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

void tw_set_256(tw_u256* y, tw_u256* a) {
  for (int i = 0; i < 8; i++) {
    y->d[i] = a->d[i];
  }
}

void tw_set_32(tw_u256* y, tw_u32 a) {
  for (int i = 0; i < 7; i++) {
    y->d[i] = 0;
  }
  y->d[7] = a;
}


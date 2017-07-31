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


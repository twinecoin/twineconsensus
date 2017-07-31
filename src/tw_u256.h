#ifndef TW_U256_h
#define TW_U256_h

#include <stdint.h>

/* 256-bit unsigned integer data type
 * data[0] is MSW and data[7] is LSW
 */
typedef struct _tw_u256 {
  uint32_t d[8];
} tw_u256;

/* Shorter type names */
typedef uint32_t tw_u32;
typedef int32_t tw_i32;
typedef uint64_t tw_u64;
typedef int64_t tw_i64;

/* Equality check
 * Returns 1 if equal
 */
int tw_equal(tw_u256* a, tw_u256* b);

/* Addition with overflow
 * y = a + b
 * Returns 1 on overflow
 */
int tw_add(tw_u256* y, tw_u256* a, tw_u256* b);

/* Subtraction with borrow
 * y = a - b
 * Returns 1 on borrow
 */
int tw_sub(tw_u256* y, tw_u256* a, tw_u256* b);

#endif

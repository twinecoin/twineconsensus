/*               Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_U256_h
#define TW_U256_h

#include <stdint.h>

/* Shorter type names */
typedef uint32_t tw_u32;
typedef int32_t tw_i32;
typedef uint64_t tw_u64;
typedef int64_t tw_i64;

/* 256-bit unsigned integer data type
 * data[0] is MSW and data[7] is LSW
 */
typedef struct _tw_u256 {
  tw_u32 d[8];
} tw_u256;

/* Some constants */
extern const tw_u256 TW_ZERO;
extern const tw_u256 TW_ONE;

#define TW_U64_MAX (0xFFFFFFFFFFFFFFFFL)
#define TW_U32_MAX (0xFFFFFFFF)

/* Equality check
 * Returns 1 if equal
 */
int tw_equal(const tw_u256* a, const tw_u256* b);

/* Comparison
 * returns -1 if (a < b), 1 if (a > b) and zero if equal
 */
int tw_compare(const tw_u256* a, const tw_u256* b);

/* Addition with carry
 * y = a + b
 * Returns 1 on carry
 */
int tw_add(tw_u256* y, const tw_u256* a, const tw_u256* b);

/* Addition by a u32 with word left shift
 * y = a + (b << (32 * (left_shift & 7)))
 * Returns 1 on carry
 */
int tw_add_32(tw_u256* y, const tw_u256* a, const tw_u32 b, const tw_u32 left_shift);

/* Subtraction with borrow
 * y = a - b
 * Returns 1 on borrow
 */
int tw_sub(tw_u256* y, const tw_u256* a, const tw_u256* b);

/* Subtraction by a u32 with word left shift
 * y = a - (b << (32 * (left_shift & 7)))
 * Returns 1 on borrow
 */
int tw_sub_32(tw_u256* y, const tw_u256* a, const tw_u32 b, const tw_u32 left_shift);

/* Multiply by a u32 with word left_shift
 * y = a * (b << (32 * (left_shift & 7)))
 * Returns 1 on overflow
 */
int tw_mul_32(tw_u256* y, const tw_u256* a, const tw_u32 b, const tw_u32 left_shift);

/* Multiply with overflow
 * y = a * b
 * returns 1 on overflow
 */
int tw_mul(tw_u256* y, const tw_u256* a, const tw_u256* b);

/* Divide and remainder with zero check
 * y = a / b
 * z = a % b
 * returns 1 for divide by zero
 */
int tw_div_rem(tw_u256* y, tw_u256* z, const tw_u256* a, const tw_u256* b);

/* Set to a tw_u256
 * y = a
 */
void tw_set_256(tw_u256* y, const tw_u256* a);

/* Set to a tw_u32
 * y = (tw_u256) a
 */
void tw_set_32(tw_u256* y, const tw_u32 a);

#endif

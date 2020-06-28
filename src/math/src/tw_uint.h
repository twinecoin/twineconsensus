/*               Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_UINT_H
#define TW_UINT_H

/* This is an implementation of 512-bit fixed size integer maths.
 * Conversion to 256-bit and 64-bit integers is supported
 */

#include <stdint.h>

/* Shorter type names */
typedef uint8_t tw_u8;
typedef int8_t tw_i8;
typedef uint32_t tw_u32;
typedef int32_t tw_i32;
typedef uint64_t tw_u64;
typedef int64_t tw_i64;

/* 512-bit unsigned integer data type
 * Each limb is 64-bits and the array is little endian.
 * data[0] is LSW and data[7] is MSW
 */
typedef struct _tw_u512 {
  tw_u64 d[8];
} tw_u512;

/* Some constants */
extern const tw_u512 TW_UINT_ZERO;
extern const tw_u512 TW_UINT_ONE;
extern const tw_u512 TW_UINT_MAX;
extern const tw_u512 TW_UINT256_MAX;

#define TW_U64_MAX (0xFFFFFFFFFFFFFFFFLLU)
#define TW_U32_MAX (0xFFFFFFFFLU)

/* Set to a tw_u512
 * y = a
 */
void tw_set_512(tw_u512* y, const tw_u512* a);

/* Set to a tw_u64
 * y = (tw_u512) a
 */
void tw_set_64(tw_u512* y, const tw_u64 a);

/* Equality check
 * Returns 1 if equal
 */
int tw_equal(const tw_u512* a, const tw_u512* b);

/* Comparison
 * returns -1 if (a < b), 1 if (a > b) and zero if equal
 */
int tw_compare(const tw_u512* a, const tw_u512* b);

/* Addition with carry
 * y = a + b
 * Returns 1 on carry
 */
int tw_add(tw_u512* y, const tw_u512* a, const tw_u512* b);

/* Addition by a u64 with 32-bit left shift
 * y = a + (b << (32 * (left_shift & 15)))
 * Returns 1 on carry
 */
int tw_add_32_lshift(tw_u512* y, const tw_u512* a, const tw_u64 b, const tw_u32 left_shift);

/* Subtraction with borrow
 * y = a - b
 * Returns 1 on borrow
 */
int tw_sub(tw_u512* y, const tw_u512* a, const tw_u512* b);

/* Subtraction by a u64 with 32-bit left shift
 * y = a - (b << (32 * (left_shift & 15)))
 * Returns 1 on borrow
 */
int tw_sub_32_lshift(tw_u512* y, const tw_u512* a, const tw_u64 b, const tw_u32 left_shift);

/* Multiply with overflow
 * y = a * b
 * returns 1 on overflow
 */
int tw_mul(tw_u512* y, const tw_u512* a, const tw_u512* b);

#endif

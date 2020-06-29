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

/* Floating point representation of a tw_512 integer
 * The word exponent represents 32-bit shifts
 * The mantissa is a tw_u64 and at least one of the top-32 bits
 * is non-zero, except when representing zero
 * value = man << (32 * w_exp + b_exp)
 */
typedef struct _tw_u64_float {
  tw_u64 man;    // Mantissa
  tw_i32 w_exp;  // 32-bit word exponent
  tw_i32 b_exp;  // bit exponent
} tw_u64_float;

/* Some constants */
extern const tw_u512 TW_UINT_ZERO;
extern const tw_u512 TW_U512_ONE;
extern const tw_u512 TW_U512_MAX;

#define TW_U64_MAX     (0xFFFFFFFFFFFFFFFFLLU)
#define TW_U64_SET_B16 (0x0000000000010000LLU)
#define TW_U64_SET_B32 (0x0000000100000000LLU)
#define TW_U64_SET_B48 (0x0001000000000000LLU)
#define TW_U64_SET_B63 (0x8000000000000000LLU)
#define TW_U32_MAX     (0xFFFFFFFFLU)

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

/* Multiplication by a u64 with 32-bit left shift
 * y = a * (b << (32 * (left_shift & 15)))
 * Returns 1 on overflow
 */
int tw_mul_32_lshift(tw_u512* y, const tw_u512* a, const tw_u64 b, const tw_u32 left_shift);

/* Converts a tw_u512 into a tw_u64_float
 * All rounding is downwards.
 * If b_exp is negative then it is selected automatically so that the
 * the mantissa's msb is bit 63.
 * When representing zero, a b_exp of zero is selected.
 *
 * returns (tw_u64_float) a with b_exp = (b_exp & 31)
 */
tw_u64_float u512_to_u64_float(const tw_u512* a, tw_i32 b_exp);

#endif

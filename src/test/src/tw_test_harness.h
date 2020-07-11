/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_TEST_HARNESS_H
#define TW_TEST_HARNESS_H

#include "tw_uint.h"

/* Test Harness
 * WARNING: The functions in this file are for test use only
 *          and may be modified without notice
 */

#define TW_TEST_HARNESS_UNLOCK_CODE (0x14e69cc2962eeeb7UL)

/* Unlocks the test harness
 * This function can be used to lock and unlock the test harness
 * The functions in the harness have no effect if the harness is locked
 * and return 0
 * The harness is locked by default
 * Returns the old value of the locked bit
 */
int tw_th_unlock_test_functions(tw_u64 code, int unlock);

// tw_u512 test harness access functions for the corresponding name

// tw_u512 functions
int tw_th_equal(const tw_u512* a, const tw_u512* b);
int tw_th_compare(const tw_u512* a, const tw_u512* b);
int tw_th_add(tw_u512* y, const tw_u512* a, const tw_u512* b);
int tw_th_sub(tw_u512* y, const tw_u512* a, const tw_u512* b);
int tw_th_lshift(tw_u512* y, const tw_u512*a, const tw_u32 bitshift);
int tw_th_rshift(tw_u512* y, const tw_u512*a, const tw_u32 bitshift);
int tw_th_mul(tw_u512* y, const tw_u512* a, const tw_u512* b);
int tw_th_div_rem(tw_u512* y, tw_u512* z, const tw_u512* a, const tw_u512* b);

// tw_hash functions
tw_u512 tw_th_sha_256(const tw_u8* message, const tw_u64 len);
tw_u512 tw_th_dsha_256(const tw_u8* message, const tw_u64 len);
tw_u512 tw_th_ripemd_160(const tw_u8* message, const tw_u64 len);
tw_u512 tw_th_sha_512(const tw_u8* message, const tw_u64 len);
tw_u512 tw_th_dsha_512(const tw_u8* message, const tw_u64 len);

// tw_signature_verify functions
int tw_th_checksig_ed25519(tw_u8* message, tw_u32* message_length,
                           const tw_u8* signature, const tw_u32 signature_length,
                           const tw_u8* public_key);


#endif

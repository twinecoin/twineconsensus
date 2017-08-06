/*               Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_SHA256_H
#define TW_SHA256_H

/* Wrapper around secp256k1 SHA256 implementation
*/

#include "hash_impl.h"
#include "tw_u256.h"

/* 32 byte hash value
 */
typedef struct _tw_h256 {
  tw_u8 h[32];
} tw_h256;

/* SHA256 hash state */
typedef secp256k1_sha256_t tw_sha256_state;

/* Check if two tw_h256 values are equal
 * Returns 1 if equal
 */
int tw_h256_equal(const tw_h256* a, const tw_h256* b);

/* Get initialized SHA256 hash state
 * Returns an initialized hash state
 */
tw_sha256_state tw_create_sha256_state();

/* Reset/Initialize a SHA256 hash state
 * sha256_state.reset()
 */
void tw_sha256_reset(tw_sha256_state* sha256_state);

/* Write data to SHA256 hash state
 * sha256_state.write(data, size)
 */
void tw_sha256_write(tw_sha256_state* sha256_state, const tw_u8 *data, const size_t size);

/* Finalize SHA256 hash state
 * out = sha256_state.result()
 */
void tw_sha256_done(tw_sha256_state* sha256_state, tw_h256* out);

/* Compute the SHA256 hash of a block of data
 * out = sha256(data, size);
 */
void tw_sha256_hash(tw_h256* out, const tw_u8 *data, const size_t size);

#endif

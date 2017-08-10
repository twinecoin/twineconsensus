/*               Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_SECP256K1_H
#define TW_SECP256K1_H

#include "../secp256k1/include/secp256k1.h"

/* Wrapper around secp256k1 library
*/

#include "tw_u256.h"
#include "tw_sha256.h"

/* Secp256k1 decoded public key */
typedef secp256k1_pubkey tw_pubkey;

/* Secp256k1 decoded signature */
typedef secp256k1_ecdsa_signature tw_signature;

/* Secp256k1 verification context */
typedef struct tw_verify_context_ {
  secp256k1_context* secp256k1_ctx;
  const char* illegal_message;
  tw_u8 illegal_flag;
} tw_verify_context;

/* Create a verify context
 * Returns the verify_context
 */
tw_verify_context tw_create_verify_context();

/* Destroys a verify context
 * This method may not be called twice on the same context
 * success = ctx->destroy()
 */
int tw_destroy_verify_context(tw_verify_context* ctx);

/* Tests if a context is valid for use
 * valid = tw_test_valid(ctx)
 */
int tw_test_valid(tw_verify_context* ctx);

/* Parse an encoded public key
 * Note: decode failures can leave the pub_key in an unknown state
 * success = tw_decode_pub(pub_key, encoded, size)
 */
int tw_decode_pub(tw_verify_context* ctx, tw_pubkey* pub_key, const tw_u8 *encoded, const size_t size);

/* Parse an encoded signature
 * success = tw_decode_signature(sig, encoded, size)
 */
int tw_decode_sig(tw_verify_context* ctx, tw_signature* sig, const tw_u8 *encoded, const size_t size);

/* Verify a signature
 * Fails if the s value is not a low-s value (<= order / 2)
 * success = tw_verify(tw_verify_context* ctx, pub_key, sig, message);
 */
int tw_verify(tw_verify_context* ctx, const tw_pubkey* pub_key, const tw_signature* sig, const tw_h256* message);

#endif

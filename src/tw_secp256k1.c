/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <stdio.h>

#include "tw_secp256k1.h"

int check_illegal_flag(tw_verify_context* ctx) {
  if (ctx->illegal_flag) {
    ctx->illegal_flag = 0;
    return 0;
  }
  return 1;
}

void secp256k1_illegal_callback(const char* message, void* data);
void secp256k1_error_callback(const char* message, void* data);

void secp256k1_illegal_callback(const char* message, void* data) {
  tw_verify_context* ctx = (tw_verify_context*) data;
  ctx->illegal_flag = 1;
  ctx->illegal_message = message;
}

void secp256k1_error_callback(const char* message, void* data) {
  fprintf(stderr, "Fatal error with secp256k1 library: %s", message);
  abort();
}

tw_verify_context tw_create_verify_context() {
  tw_verify_context ctx;
  ctx.secp256k1_ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
  ctx.illegal_flag = 0;
  ctx.illegal_message = NULL;
  if (ctx.secp256k1_ctx != NULL) {
    secp256k1_context_set_illegal_callback(ctx.secp256k1_ctx, secp256k1_illegal_callback, &ctx);
    secp256k1_context_set_error_callback(ctx.secp256k1_ctx, secp256k1_error_callback, &ctx);
  }
  return ctx;
}

int tw_destroy_verify_context(tw_verify_context* ctx) {
  if (ctx->secp256k1_ctx != NULL) {
    secp256k1_context_destroy(ctx->secp256k1_ctx);
    ctx->secp256k1_ctx = NULL;
    return check_illegal_flag(ctx);
  }
  return 0;
}

int tw_test_valid(tw_verify_context* ctx) {
  return ctx->secp256k1_ctx != NULL;
}

int tw_decode_pub(tw_verify_context* ctx, tw_pubkey* pub_key, const tw_u8 *encoded, const size_t size) {
  if (size > 0 && (encoded[0] == 0x06 || encoded[0] == 0x07)) {
    return 0;
  }
  int success = secp256k1_ec_pubkey_parse(ctx->secp256k1_ctx, pub_key, encoded, size);
  if (!success) {
    return 0;
  }
  return check_illegal_flag(ctx);
}

int tw_decode_sig(tw_verify_context* ctx, tw_signature* sig, const tw_u8 *encoded, const size_t size) {
  int success = secp256k1_ecdsa_signature_parse_der(ctx->secp256k1_ctx, sig, encoded, size);
  if (!success) {
    return 0;
  }
  return check_illegal_flag(ctx);
}

#include <stdio.h>
int tw_verify(tw_verify_context* ctx, const tw_pubkey* pub_key, const tw_signature* sig, const tw_h256* message) {
  int success = secp256k1_ecdsa_verify(ctx->secp256k1_ctx, sig, message->h, pub_key);
  if (!success) {
    return 0;
  }
  return check_illegal_flag(ctx);
}

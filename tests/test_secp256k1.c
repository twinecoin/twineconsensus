/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/tw_secp256k1.h"

#include "vectors/vectors_secp256k1.h"

/* Create a verify context
 * Returns the verify_context
 */
tw_verify_context tw_create_verify_context();

/* Destroys a verify context
 * This method may not be called twice on the same context
 * success = ctx->destroy()
 */
int tw_destroy_verify_context(tw_verify_context* ctx);

/* Parse an encoded public key
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

// Double destory test

START_TEST (test_tw_destroy_verify_context) {
  tw_verify_context ctx = tw_create_verify_context();
  ck_assert_msg(tw_test_valid(&ctx), "Failed to create context");

  ck_assert_msg(tw_destroy_verify_context(&ctx), "Failed to cleanly destroy context");

  ck_assert_msg(!tw_destroy_verify_context(&ctx), "No error when destroying a destroyed context");
}
END_TEST

// Public Key Decode Test

START_TEST (test_tw_decode_pub) {
  tw_verify_context ctx = tw_create_verify_context();
  ck_assert_msg(tw_test_valid(&ctx), "Failed to create context");

  for (int i = 0; i < SECP256K1_TEST_VECTORS_LENGTH; i += SECP256K1_TEST_VECTORS_STEP) {
    tw_u8** base = &(tw_secp256k1_test_vector[i]);
    tw_u8 sig_valid = **(base + 0);
    tw_u8 sig_valid_encode = **(base + 1);
    tw_u8 pub_key_valid = **(base + 2);
    tw_u8 sig_len = **(base + 3);
    tw_u8 pub_key_len = **(base + 4);
    tw_u8* sig = *(base + 5);
    tw_u8* pub_key = *(base + 6);
    tw_u8* message_hash = *(base + 7);

    tw_pubkey pub_key_decoded;
    ck_assert_msg(tw_decode_pub(&ctx, &pub_key_decoded, pub_key, pub_key_len) == pub_key_valid, "Public key decode valid check failure for vector %d",
                                      i / SECP256K1_TEST_VECTORS_STEP);
  }

  ck_assert_msg(tw_destroy_verify_context(&ctx), "Failed to cleanly destroy context");
}
END_TEST

// Signature Decode Test

START_TEST (test_tw_decode_sig) {
  tw_verify_context ctx = tw_create_verify_context();
  ck_assert_msg(tw_test_valid(&ctx), "Failed to create context");

  for (int i = 0; i < SECP256K1_TEST_VECTORS_LENGTH; i += SECP256K1_TEST_VECTORS_STEP) {
    tw_u8** base = &(tw_secp256k1_test_vector[i]);
    tw_u8 sig_valid = **(base + 0);
    tw_u8 sig_valid_encode = **(base + 1);
    tw_u8 pub_key_valid = **(base + 2);
    tw_u8 sig_len = **(base + 3);
    tw_u8 pub_key_len = **(base + 4);
    tw_u8* sig = *(base + 5);
    tw_u8* pub_key = *(base + 6);
    tw_u8* message_hash = *(base + 7);

    tw_signature signature_decoded;
    ck_assert_msg(tw_decode_sig(&ctx, &signature_decoded, sig, sig_len) == sig_valid_encode, "Signature valid decode check failure for vector %d",
                                      i / SECP256K1_TEST_VECTORS_STEP);
  }

  ck_assert_msg(tw_destroy_verify_context(&ctx), "Failed to cleanly destroy context");
}
END_TEST

// Signature Test

#include <stdio.h>

START_TEST (test_tw_verify) {
  tw_verify_context ctx = tw_create_verify_context();
  ck_assert_msg(tw_test_valid(&ctx), "Failed to create context");

  for (int i = 0; i < SECP256K1_TEST_VECTORS_LENGTH; i += SECP256K1_TEST_VECTORS_STEP) {
    tw_u8** base = &(tw_secp256k1_test_vector[i]);
    tw_u8 sig_valid = **(base + 0);
    tw_u8 sig_valid_encode = **(base + 1);
    tw_u8 pub_key_valid = **(base + 2);
    tw_u8 sig_len = **(base + 3);
    tw_u8 pub_key_len = **(base + 4);
    tw_u8* sig = *(base + 5);
    tw_u8* pub_key = *(base + 6);
    tw_u8* message_bytes = *(base + 7);

    tw_h256 message_hash;
    memcpy(message_hash.h, message_bytes, 32);

    tw_pubkey pub_key_decoded;
    ck_assert_msg(tw_decode_pub(&ctx, &pub_key_decoded, pub_key, pub_key_len) == pub_key_valid, "Public key decode valid check failure for vector %d",
                                      i / SECP256K1_TEST_VECTORS_STEP);

    tw_signature signature_decoded;
    ck_assert_msg(tw_decode_sig(&ctx, &signature_decoded, sig, sig_len) == sig_valid_encode, "Signature valid decode check failure for vector %d",
                                      i / SECP256K1_TEST_VECTORS_STEP);

    if (pub_key_valid) {
      ck_assert_msg(tw_verify(&ctx, &pub_key_decoded, &signature_decoded, &message_hash) == sig_valid,
                                      "Signature verify check failure for vector %d",
                                      i / SECP256K1_TEST_VECTORS_STEP);
    }
  }

  ck_assert_msg(tw_destroy_verify_context(&ctx), "Failed to cleanly destroy context");
}
END_TEST


Suite * version_check_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Secp256k1 Test");

  /* Core test case */
  tc_core = tcase_create("Check for tw_secp256k1");

  tcase_add_test(tc_core, test_tw_destroy_verify_context);
  tcase_add_test(tc_core, test_tw_decode_pub);
  tcase_add_test(tc_core, test_tw_decode_sig);
  tcase_add_test(tc_core, test_tw_verify);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = version_check_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

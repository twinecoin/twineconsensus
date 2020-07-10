/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "tw_test_harness.h"
#include "vectors/vectors_ed25519.h"

#define MAX_BUFFER 1000000

void unlock_test_harness(int unlock) {
    tw_th_unlock_test_functions(TW_TEST_HARNESS_UNLOCK_CODE, unlock);
}

void test_locked(char* message) {
  tw_u32 message_length_out;
  int valid = tw_th_checksig_ed25519(NULL, &message_length_out, NULL, 0, NULL);
  ck_assert_msg(valid == 0, "Test harness enabled %s for tw_th_checksig_ed25519", message);
}

START_TEST(test_tw_th_unlock_test_functions) {
  tw_u512 zero = {0};

  test_locked("by default");

  // Verify re-locks after an unlock
  unlock_test_harness(1);
  unlock_test_harness(0);

  test_locked("after re-lock");

  // Verify unlock fails with incorrect code
  tw_th_unlock_test_functions(TW_TEST_HARNESS_UNLOCK_CODE ^ 0x0040000000000000ULL, 1);

  test_locked("after bad code");
}
END_TEST

START_TEST (test_tw_signature_verify) {
  unlock_test_harness(1);

  tw_u8 signature_buffer[MAX_BUFFER];
  tw_u8 message_out[MAX_BUFFER];

  for (int i = 0; i < ED25519_TEST_VECTORS_LENGTH; i++) {
    tw_u32 message_length = tw_ed25519_test_vector_message_lengths[i];
    tw_u8* message = tw_ed25519_test_vector_messages[i];
    tw_u32 signature_length = tw_ed25519_test_vector_signature_lengths[i];
    tw_u8* signature = tw_ed25519_test_vector_signatures[i];
    tw_u8* public_key_der = tw_ed25519_test_vector_public_keys[i];
    tw_u32 exp_valid = tw_ed25519_test_vector_signature_valid[i];
    memcpy(signature_buffer, signature, signature_length);
    memcpy(signature_buffer + signature_length, message, message_length);
    tw_u8 public_key[64];
    memcpy(public_key, public_key_der + 2, 64);
    tw_u32 message_length_out;
    int valid = tw_th_checksig_ed25519(message_out, &message_length_out, signature_buffer, signature_length + message_length, public_key);
    ck_assert_msg(valid == exp_valid, "Unexpected result for signature check for vector %d", i);
    if (exp_valid == 0) {
      ck_assert_msg(message_length_out == message_length, "Invalid output message length for vector %d", i);
    }
  }
}
END_TEST

Suite * uint_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Signature Verify Suite");

  /* Core test case */
  tc_core = tcase_create("Signature Verify Tests");

  tcase_add_test(tc_core, test_tw_th_unlock_test_functions);
  tcase_add_test(tc_core, test_tw_signature_verify);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = uint_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/tw_sha256.h"

#include "vectors/vectors_sha256.h"

// Hash 256 tests

START_TEST (test_tw_h256_equal) {
  for (int i = 0; i < H256_TEST_VECTORS_LENGTH; i++) {
    tw_h256 a = h256_test_vectors[i].a;
    tw_h256 b = h256_test_vectors[i].b;
    int equal = h256_test_vectors[i].a_equal_b;
    ck_assert_msg(tw_h256_equal(&a, &b) == equal, "Equality check failure for vector %d", i);
  }
}
END_TEST

// SHA256 tests

START_TEST (test_tw_sha256_single_call) {
  tw_sha256_state hash_state = tw_create_sha256_state();

  for (int i = 0; i < SHA256_TEST_VECTORS_LENGTH; i++) {
    tw_u8* m = tw_h256_test_vector_messages[i];
    int m_len = tw_h256_test_vector_message_lengths[i];
    tw_h256 h = tw_h256_test_vector_hashes[i];

    tw_h256 out;

    tw_sha256_reset(&hash_state);
    tw_sha256_write(&hash_state, m, m_len);
    tw_sha256_done(&hash_state, &out);

    ck_assert_msg(tw_h256_equal(&h, &out), "Hash check failure for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_sha256_multiple_calls) {
  tw_sha256_state hash_state = tw_create_sha256_state();

  for (int i = 0; i < SHA256_TEST_VECTORS_LENGTH; i++) {
    tw_u8* m = tw_h256_test_vector_messages[i];
    int m_len = tw_h256_test_vector_message_lengths[i];
    tw_h256 h = tw_h256_test_vector_hashes[i];

    tw_h256 out;

    tw_sha256_reset(&hash_state);

    int j = 0;
    while (m_len > 0) {
      if (j > m_len) {
        j = m_len;
      }
      tw_sha256_write(&hash_state, m, j);
      m_len -= j;
      m += j;
      j++;
    }

    tw_sha256_done(&hash_state, &out);

    ck_assert_msg(tw_h256_equal(&h, &out), "Hash check failure for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_sha256_combined_call) {
  for (int i = 0; i < SHA256_TEST_VECTORS_LENGTH; i++) {
    tw_u8* m = tw_h256_test_vector_messages[i];
    int m_len = tw_h256_test_vector_message_lengths[i];
    tw_h256 h = tw_h256_test_vector_hashes[i];

    tw_h256 out;

    tw_sha256_hash(&out, m, m_len);

    ck_assert_msg(tw_h256_equal(&h, &out), "Hash check failure for vector %d", i);
  }
}
END_TEST

Suite * version_check_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("SHA256 Test");

  /* Core test case */
  tc_core = tcase_create("Check for tw_sha256");

  tcase_add_test(tc_core, test_tw_h256_equal);
  tcase_add_test(tc_core, test_tw_sha256_single_call);
  tcase_add_test(tc_core, test_tw_sha256_multiple_calls);
  tcase_add_test(tc_core, test_tw_sha256_combined_call);
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

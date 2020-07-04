/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "tw_uint.h"
#include "tw_hash.h"
#include "tw_test_harness.h"
#include "vectors/vectors_u512.h"
#include "vectors/vectors_hash.h"

#define MAX_BUFFER 1000000

#include <stdio.h>

void unlock_test_harness(int unlock) {
    tw_th_unlock_test_functions(TW_TEST_HARNESS_UNLOCK_CODE, unlock);
}

int repeat_message(tw_u8* out, tw_u8* message, int len, int rep) {
  if (len * rep > MAX_BUFFER) {
    return 0;
  }
  for (int i = 0; i < rep; i++) {
    memcpy(out + i * len, message, len * sizeof(*out));
  }
  return 1;
}

// Test if two u512 integers are equal.
// Alternative to tw_equals when testing the test harness
int test_u512_equal(tw_u512 a, tw_u512 b) {
  for (int i = 0; i < 8; i++) {
    if (a.d[i] != b.d[i]) {
      return 0;
    }
  }
  return 1;
}

void test_locked(char* message) {
  tw_u512 zero = {0};

  tw_u512 digest = tw_th_sha_256(NULL, 0);
  ck_assert_msg(test_u512_equal(digest, zero), "Test harness enabled %s for tw_th_sha_256", message);
  digest = tw_th_dsha_256(NULL, 0);
  ck_assert_msg(test_u512_equal(digest, zero), "Test harness enabled %s for tw_th_dsha_256", message);
  digest = tw_th_sha_512(NULL, 0);
  ck_assert_msg(test_u512_equal(digest, zero), "Test harness enabled %s for tw_th_sha_512", message);
  digest = tw_th_dsha_512(NULL, 0);
  ck_assert_msg(test_u512_equal(digest, zero), "Test harness enabled %s for tw_th_dsha_512", message);
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

START_TEST (test_tw_sha_256) {
  unlock_test_harness(1);

  tw_u8 buffer[MAX_BUFFER];

  for (int i = 0; i < HASH_TEST_VECTORS_LENGTH; i++) {
    tw_u512 expected = tw_sha256_test_vector_hashes[i];
    tw_u8* message = tw_hash_test_vector_messages[i];
    int len = tw_hash_test_vector_message_lengths[i];
    int rep = tw_hash_test_vector_message_repeats[i];
    ck_assert_msg(repeat_message(buffer, message, len, rep), "Failed to repeat message for SHA256 Check");
    tw_u512 digest = tw_th_sha_256(buffer, len * rep);
    ck_assert_msg(tw_th_equal(&digest, &expected), "Hash mismatch for SHA256 check for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_dsha_256) {
  unlock_test_harness(1);

  tw_u8 buffer[MAX_BUFFER];

  for (int i = 0; i < HASH_TEST_VECTORS_LENGTH; i++) {
    tw_u512 expected = tw_dsha256_test_vector_hashes[i];
    tw_u8* message = tw_hash_test_vector_messages[i];
    int len = tw_hash_test_vector_message_lengths[i];
    int rep = tw_hash_test_vector_message_repeats[i];
    ck_assert_msg(repeat_message(buffer, message, len, rep), "Failed to repeat message for SHA256 Check");
    tw_u512 digest = tw_th_dsha_256(buffer, len * rep);
    ck_assert_msg(tw_th_equal(&digest, &expected), "Hash mismatch for SHA256 check for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_sha_512) {
  unlock_test_harness(1);

  tw_u8 buffer[MAX_BUFFER];

  for (int i = 0; i < HASH_TEST_VECTORS_LENGTH; i++) {
    tw_u512 expected = tw_sha512_test_vector_hashes[i];
    tw_u8* message = tw_hash_test_vector_messages[i];
    int len = tw_hash_test_vector_message_lengths[i];
    int rep = tw_hash_test_vector_message_repeats[i];
    ck_assert_msg(repeat_message(buffer, message, len, rep), "Failed to repeat message for SHA512 Check");
    tw_u512 digest = tw_th_sha_512(buffer, len * rep);
    ck_assert_msg(tw_th_equal(&digest, &expected), "Hash mismatch for SHA512 check for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_dsha_512) {
  unlock_test_harness(1);

  tw_u8 buffer[MAX_BUFFER];

  for (int i = 0; i < HASH_TEST_VECTORS_LENGTH; i++) {
    tw_u512 expected = tw_dsha512_test_vector_hashes[i];
    tw_u8* message = tw_hash_test_vector_messages[i];
    int len = tw_hash_test_vector_message_lengths[i];
    int rep = tw_hash_test_vector_message_repeats[i];
    ck_assert_msg(repeat_message(buffer, message, len, rep), "Failed to repeat message for SHA512 Check");
    tw_u512 digest = tw_th_dsha_512(buffer, len * rep);
    ck_assert_msg(tw_th_equal(&digest, &expected), "Hash mismatch for SHA512 check for vector %d", i);
  }
}
END_TEST

Suite * uint_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Hash Test Suite");

  /* Core test case */
  tc_core = tcase_create("Hash Tests");

  tcase_add_test(tc_core, test_tw_th_unlock_test_functions);
  tcase_add_test(tc_core, test_tw_sha_256);
  tcase_add_test(tc_core, test_tw_dsha_256);
  tcase_add_test(tc_core, test_tw_sha_512);
  tcase_add_test(tc_core, test_tw_dsha_512);
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

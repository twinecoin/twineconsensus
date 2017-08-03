/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/tw_u256.h"

#include "vectors/vectors_u256.h"

// u256xu256 Binary Tests

START_TEST (test_tw_equal) {
  tw_u256 a, b;
  for (int i = 0; i < U256_TEST_VECTORS_256X256_LENGTH; i++) {
    a = u256_test_vectors_256x256[i].a;
    b = u256_test_vectors_256x256[i].b;
    int equal = u256_test_vectors_256x256[i].a_equal_b;
    ck_assert_msg(tw_equal(&a, &b) == equal, "Equality check failure for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_add) {
  tw_u256 a, b, y;
  for (int i = 0; i < U256_TEST_VECTORS_256X256_LENGTH; i++) {
    a = u256_test_vectors_256x256[i].a;
    b = u256_test_vectors_256x256[i].b;
    int carry = u256_test_vectors_256x256[i].a_add_b_carry;
    ck_assert_msg(tw_add(&y, &a, &b) == carry, "Carry mismatch for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u256_test_vectors_256x256[i].a_add_b), "Addition mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_sub) {
  tw_u256 a, b, y;
  for (int i = 0; i < U256_TEST_VECTORS_256X256_LENGTH; i++) {
    a = u256_test_vectors_256x256[i].a;
    b = u256_test_vectors_256x256[i].b;
    int borrow = u256_test_vectors_256x256[i].a_sub_b_borrow;
    ck_assert_msg(tw_sub(&y, &a, &b) == borrow, "Borrow mismatch for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u256_test_vectors_256x256[i].a_sub_b), "Subtraction mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_mul) {
  tw_u256 a, b, y;
  for (int i = 0; i < U256_TEST_VECTORS_256X256_LENGTH; i++) {
    a = u256_test_vectors_256x256[i].a;
    b = u256_test_vectors_256x256[i].b;
    int overflow = u256_test_vectors_256x256[i].a_mul_b_overflow;
    ck_assert_msg(tw_mul(&y, &a, &b) == overflow, "Overflow mismatch for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u256_test_vectors_256x256[i].a_mul_b), "Multiplication mismatch for vector %d", i);
  }
}
END_TEST

// u256xu32 Binary Tests

START_TEST (test_tw_mul_32) {
  tw_u256 a, y;
  tw_u32 b;
  tw_u32 shift;
  for (int i = 0; i < U256_TEST_VECTORS_256X32_LENGTH; i++) {
    a = u256_test_vectors_256x32[i].a;
    b = u256_test_vectors_256x32[i].b;
    shift = u256_test_vectors_256x32[i].s;
    int overflow = u256_test_vectors_256x32[i].a_mul_b_overflow;
    ck_assert_msg(tw_mul_32(&y, &a, b, shift) == overflow, "Mul32 overflow mismatch for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u256_test_vectors_256x32[i].a_mul_b), "Mul32 mismatch for vector %d", i);
  }
}
END_TEST

// u256 Unary Tests

START_TEST (test_tw_set) {
  tw_u256 a, y;
  tw_u256 target = {0, 0, 0, 0, 0, 0, 0, 0};
  tw_u32 b;
  for (int i = 0; i < U256_TEST_VECTORS_256X32_LENGTH; i++) {
    a = u256_test_vectors_256x32[i].a;
    b = u256_test_vectors_256x32[i].b;
    tw_set_256(&y, &a);
    ck_assert_msg(tw_equal(&y, &u256_test_vectors_256x32[i].a), "Set 256 mismatch for vector %d", i);
    target.d[7] = b;
    tw_set_32(&y, b);
    ck_assert_msg(tw_equal(&y, &target), "Set 32 mismatch for vector %d", i);
  }
}
END_TEST

Suite * version_check_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("U256 Maths Test");

  /* Core test case */
  tc_core = tcase_create("Check for tw_u256");

  tcase_add_test(tc_core, test_tw_equal);
  tcase_add_test(tc_core, test_tw_add);
  tcase_add_test(tc_core, test_tw_sub);
  tcase_add_test(tc_core, test_tw_mul_32);
  tcase_add_test(tc_core, test_tw_mul);
  tcase_add_test(tc_core, test_tw_set);
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

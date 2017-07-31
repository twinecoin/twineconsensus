#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/tw_u256.h"

#include "vectors/vectors_u256.h"

START_TEST (test_tw_equal) {
  tw_u256 a, b;
  for (int i = 0; i < U256_TEST_VECTORS_LENGTH; i++) {
    a = u256_test_vectors[i].a;
    b = u256_test_vectors[i].b;
    int equal = u256_test_vectors[i].a_equal_b;
    ck_assert_msg(tw_equal(&a, &b) == equal, "Equality check failure");
  }
}
END_TEST

START_TEST (test_tw_add) {
  tw_u256 a, b, y;
  for (int i = 0; i < U256_TEST_VECTORS_LENGTH; i++) {
    a = u256_test_vectors[i].a;
    b = u256_test_vectors[i].b;
    int carry = u256_test_vectors[i].a_add_b_carry;
    ck_assert_msg(tw_add(&y, &a, &b) == carry, "Carry mismatch");
    ck_assert_msg(tw_equal(&y, &u256_test_vectors[i].a_add_b), "Addition mismatch");
  }
}
END_TEST

START_TEST (test_tw_sub) {
  tw_u256 a, b, y;
  for (int i = 0; i < U256_TEST_VECTORS_LENGTH; i++) {
    a = u256_test_vectors[i].a;
    b = u256_test_vectors[i].b;
    int borrow = u256_test_vectors[i].a_sub_b_borrow;
    ck_assert_msg(tw_sub(&y, &a, &b) == borrow, "Borrow mismatch");
    ck_assert_msg(tw_equal(&y, &u256_test_vectors[i].a_sub_b), "Subtraction mismatch");
  }
}
END_TEST

Suite * version_check_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("U256 Maths Test");

  /* Core test case */
  tc_core = tcase_create("Check for tw_add");

  tcase_add_test(tc_core, test_tw_equal);
  tcase_add_test(tc_core, test_tw_add);
  tcase_add_test(tc_core, test_tw_sub);
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

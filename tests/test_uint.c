/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/math/src/tw_uint.h"
#include "vectors/vectors_u512.h"

START_TEST (test_tw_set) {
  tw_u512 a, y;
  tw_u512 expected = {0, 0, 0, 0, 0, 0, 0, 0};
  tw_u64 b;
  for (int i = 0; i < U512_TEST_VECTORS_512X64_LENGTH; i++) {
    a = u512_test_vectors_512x64[i].a;
    b = u512_test_vectors_512x64[i].b;
    for (int j = 0; j < 8; j++) {
      y.d[j] = a.d[j] == 0;
    }
    tw_set_512(&y, &a);
    ck_assert_msg(tw_equal(&y, &a), "512-bit set check failed for vector %d", i);
    for (int j = 0; j < 8; j++) {
      y.d[j] = 1;
    }
    tw_set_64(&y, b);
    expected.d[0] = b;
    ck_assert_msg(tw_equal(&y, &expected), "64-bit set check failed for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_equal) {
  tw_u512 a, b;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    a = u512_test_vectors_512x512[i].a;
    b = u512_test_vectors_512x512[i].b;
    int equal = u512_test_vectors_512x512[i].a_equal_b;
    ck_assert_msg(tw_equal(&a, &b) == equal, "Equality check failed for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_compare) {
  tw_u512 a, b;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    a = u512_test_vectors_512x512[i].a;
    b = u512_test_vectors_512x512[i].b;
    int compare = u512_test_vectors_512x512[i].a_comp_b;
    ck_assert_msg(tw_compare(&a, &b) == compare, "Comparison check failed for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_add) {
  tw_u512 a, b, y;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    a = u512_test_vectors_512x512[i].a;
    b = u512_test_vectors_512x512[i].b;
    int carry = u512_test_vectors_512x512[i].a_add_b_carry;
    ck_assert_msg(tw_add(&y, &a, &b) == carry, "Addition carry check failed for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x512[i].a_add_b), "Addition mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_add_32_lshift) {
  tw_u512 a, y;
  tw_u64 b;
  tw_u32 shift;
  for (int i = 0; i < U512_TEST_VECTORS_512X64_LENGTH; i++) {
    a = u512_test_vectors_512x64[i].a;
    b = u512_test_vectors_512x64[i].b;
    shift = u512_test_vectors_512x64[i].s;
    int carry = u512_test_vectors_512x64[i].a_add_b_carry;
    ck_assert_msg(tw_add_32_lshift(&y, &a, b, shift) == carry, "Addition with left shift carry check failed for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x64[i].a_add_b), "Addition with left shift mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_sub) {
  tw_u512 a, b, y;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    a = u512_test_vectors_512x512[i].a;
    b = u512_test_vectors_512x512[i].b;
    int borrow = u512_test_vectors_512x512[i].a_sub_b_borrow;
    ck_assert_msg(tw_sub(&y, &a, &b) == borrow, "Subtraction carry check failed for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x512[i].a_sub_b), "Subtraction mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_sub_32_lshift) {
  tw_u512 a, y;
  tw_u64 b;
  tw_u32 shift;
  for (int i = 0; i < U512_TEST_VECTORS_512X64_LENGTH; i++) {
    a = u512_test_vectors_512x64[i].a;
    b = u512_test_vectors_512x64[i].b;
    shift = u512_test_vectors_512x64[i].s;
    int borrow = u512_test_vectors_512x64[i].a_sub_b_borrow;
    ck_assert_msg(tw_sub_32_lshift(&y, &a, b, shift) == borrow, "Subtraction with left shift borrow check failed for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x64[i].a_sub_b), "Subtraction with left shift mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_mul) {
  tw_u512 a, b, y;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    a = u512_test_vectors_512x512[i].a;
    b = u512_test_vectors_512x512[i].b;
    int overflow = u512_test_vectors_512x512[i].a_mul_b_overflow;
    ck_assert_msg(tw_mul(&y, &a, &b) == overflow, "Multiply overflow check failed for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x512[i].a_mul_b), "Multiplication mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_mul_32_lshift) {
  tw_u512 a, y;
  tw_u64 b;
  tw_u32 shift;
  for (int i = 0; i < U512_TEST_VECTORS_512X64_LENGTH; i++) {
    a = u512_test_vectors_512x64[i].a;
    b = u512_test_vectors_512x64[i].b;
    shift = u512_test_vectors_512x64[i].s;
    int overflow = u512_test_vectors_512x64[i].a_mul_b_overflow;
    ck_assert_msg(tw_mul_32_lshift(&y, &a, b, shift) == overflow, "Multiplication with left shift overflow check failed for vector %d", i);
    ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x64[i].a_mul_b), "Multiplication with left shift mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_u512_to_u64_float) {
  tw_u512 a, y;
  tw_i32 b_exp;
  for (int i = 0; i < U512_TEST_VECTORS_512X64_LENGTH; i++) {
    a = u512_test_vectors_512x64[i].a;
    b_exp = u512_test_vectors_512x64[i].s;
    tw_u64_float a_float = u512_to_u64_float(&a, b_exp);
    tw_u64_float a_float_exp = u512_test_vectors_512x64[i].exp_float;
    ck_assert_msg(a_float.man == a_float_exp.man, "U64 float conversion mantissa check failed for vector %d", i);
    ck_assert_msg(a_float.w_exp == a_float_exp.w_exp, "U64 float conversion word exponent check failed for vector %d", i);
    ck_assert_msg(a_float.b_exp == a_float_exp.b_exp, "U64 float conversion bit exponent check failed for vector %d", i);
  }
}
END_TEST

Suite * uint_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("UINT-512 Test Suite");

  /* Core test case */
  tc_core = tcase_create("Equal test");

  tcase_add_test(tc_core, test_tw_set);
  tcase_add_test(tc_core, test_tw_equal);
  tcase_add_test(tc_core, test_tw_compare);
  tcase_add_test(tc_core, test_tw_add);
  tcase_add_test(tc_core, test_tw_add_32_lshift);
  tcase_add_test(tc_core, test_tw_sub);
  tcase_add_test(tc_core, test_tw_sub_32_lshift);
  tcase_add_test(tc_core, test_tw_mul);
  tcase_add_test(tc_core, test_tw_mul_32_lshift);
  tcase_add_test(tc_core, test_u512_to_u64_float);
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

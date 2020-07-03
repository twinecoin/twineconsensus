/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/math/src/tw_uint.h"
#include "vectors/vectors_u512.h"

// Test if two u512 integers are equal.
// Eliminates loop where tw_equal was used to verify itself
int test_u512_equal(tw_u512 a, tw_u512 b) {
  for (int i = 0; i < 8; i++) {
    if (a.d[i] != b.d[i]) {
      return 0;
    }
  }
  return 1;
}

START_TEST (test_tw_equal) {
  tw_u512 a, b;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    tw_u512 a = u512_test_vectors_512x512[i].a;
    tw_u512 b = u512_test_vectors_512x512[i].b;
    int equal = u512_test_vectors_512x512[i].a_equal_b;
    const tw_u512 a_old = a;
    const tw_u512 b_old = b;

    ck_assert_msg(tw_equal(&a, &b) == equal, "Equality check failed for vector %d", i);
    ck_assert_msg(test_u512_equal(a_old, a), "A altered for equality check for vector %d", i);
    ck_assert_msg(test_u512_equal(b_old, b), "B altered for equality check for vector %d", i);

    ck_assert_msg(tw_equal(&a, &a) != 0, "Self-equality check failed for vector %d", i);
    ck_assert_msg(test_u512_equal(a_old, a), "A altered for self-equality check for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_compare) {
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    tw_u512 a = u512_test_vectors_512x512[i].a;
    tw_u512 b = u512_test_vectors_512x512[i].b;
    int compare = u512_test_vectors_512x512[i].a_comp_b;
    const tw_u512 a_old = a;
    const tw_u512 b_old = b;

    ck_assert_msg(tw_compare(&a, &b) == compare, "Comparison check failed for vector %d", i);
    ck_assert_msg(tw_equal(&a_old, &a), "A altered for comparison check for vector %d", i);
    ck_assert_msg(tw_equal(&b_old, &b), "B altered for comparison check for vector %d", i);

    ck_assert_msg(tw_compare(&a, &a) == 0, "Self-comparison check failed for vector %d", i);
    ck_assert_msg(tw_equal(&a_old, &a), "A altered for self-comparison check for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_add) {
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    for (int j = 0; j < 5; j++) {
      int carry = u512_test_vectors_512x512[i].a_add_b_carry;
      const tw_u512 expected = u512_test_vectors_512x512[i].a_add_b;
      tw_u512 y = {0};
      tw_u512* y_ptr = &y;
      tw_u512 a = u512_test_vectors_512x512[i].a;
      tw_u512 b = u512_test_vectors_512x512[i].b;
      const tw_u512 a_old = a;
      const tw_u512 b_old = b;

      int carry_out;
      char* name;
      if (j == 0) {
        name = "y = a + b";
        y_ptr = &y;
        carry_out = tw_add(&y, &a, &b);
      } else if (j == 1) {
        y_ptr = &a;
        carry_out = tw_add(&a, &a, &b);
        name = "a = a + b";
      } else if (j == 2) {
        y_ptr = &b;
        name = "b = a + b";
        carry_out = tw_add(&b, &a, &b);
      } else if (j == 3) {
        if (!tw_equal(&a, &b)) {
          continue;
        }
        y_ptr = &y;
        name = "y = a + a";
        carry_out = tw_add(&y, &a, &a);
      } else if (j == 4) {
        if (!tw_equal(&a, &b)) {
          continue;
        }
        y_ptr = &a;
        name = "a = a + a";
        carry_out = tw_add(&a, &a, &a);
      }

      ck_assert_msg(carry_out == carry, "Carry check failed for %s check for vector %d", name, i);
      ck_assert_msg(tw_equal(y_ptr, &expected), "Addition mismatch for %s check for vector %d", name, i);
      if (j != 1 && j != 4) {
        ck_assert_msg(tw_equal(&a_old, &a), "A altered for %s check for vector %d", name, i);
      }
      if (j != 2) {
        ck_assert_msg(tw_equal(&b_old, &b), "B altered for %s check for vector %d", name, i);
      }
    }
  }
}
END_TEST

START_TEST (test_tw_sub) {
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    for (int j = 0; j < 5; j++) {
      int borrow = u512_test_vectors_512x512[i].a_sub_b_borrow;
      const tw_u512 expected = u512_test_vectors_512x512[i].a_sub_b;
      tw_u512 y = {0};
      tw_u512* y_ptr = &y;
      tw_u512 a = u512_test_vectors_512x512[i].a;
      tw_u512 b = u512_test_vectors_512x512[i].b;
      const tw_u512 a_old = a;
      const tw_u512 b_old = b;

      int borrow_out;
      char* name;
      if (j == 0) {
        name = "y = a - b";
        y_ptr = &y;
        borrow_out = tw_sub(&y, &a, &b);
      } else if (j == 1) {
        y_ptr = &a;
        borrow_out = tw_sub(&a, &a, &b);
        name = "a = a - b";
      } else if (j == 2) {
        y_ptr = &b;
        name = "b = a - b";
        borrow_out = tw_sub(&b, &a, &b);
      } else if (j == 3) {
        if (!tw_equal(&a, &b)) {
          continue;
        }
        y_ptr = &y;
        name = "y = a - a";
        borrow_out = tw_sub(&y, &a, &a);
      } else if (j == 4) {
        if (!tw_equal(&a, &b)) {
          continue;
        }
        y_ptr = &a;
        name = "a = a - a";
        borrow_out = tw_sub(&a, &a, &a);
      }

      ck_assert_msg(borrow_out == borrow, "Borrow check failed for %s check for vector %d", name, i);
      ck_assert_msg(tw_equal(y_ptr, &expected), "Subtraction mismatch for %s check for vector %d", name, i);
      if (j != 1 && j != 4) {
        ck_assert_msg(tw_equal(&a_old, &a), "A altered for %s check for vector %d", name, i);
      }
      if (j != 2) {
        ck_assert_msg(tw_equal(&b_old, &b), "B altered for %s check for vector %d", name, i);
      }
    }
  }
}
END_TEST

START_TEST (test_tw_lshift) {
  for (int i = 0; i < U512_TEST_VECTORS_512X64_LENGTH; i++) {
    tw_u512 y = {0};
    tw_u512 a = u512_test_vectors_512x64[i].a;
    tw_u32 bits = u512_test_vectors_512x64[i].s;
    tw_u512 expected = u512_test_vectors_512x64[i].a_lshift;
    int overflow = u512_test_vectors_512x64[i].a_lshift_overflow;
    const tw_u512 a_old = a;

    ck_assert_msg(tw_lshift(&y, &a, bits) == overflow, "Left-shift overflow check failed for vector %d", i);
    ck_assert_msg(tw_equal(&expected, &y), "Left-shift mismatch for vector %d", i);
    ck_assert_msg(tw_equal(&a_old, &a), "A altered for Left-shift check for vector %d", i);

    a = u512_test_vectors_512x64[i].a;
    bits = u512_test_vectors_512x64[i].s;
    expected = u512_test_vectors_512x64[i].a_lshift;
    overflow = u512_test_vectors_512x64[i].a_lshift_overflow;

    ck_assert_msg(tw_lshift(&a, &a, bits) == overflow, "Left-shift overflow self-target check failed for vector %d", i);
    ck_assert_msg(tw_equal(&expected, &a), "Left-shift self-target mismatch for vector %d", i);
  }
}
END_TEST

START_TEST (test_tw_mul) {
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    for (int j = 0; j < 5; j++) {
      int overflow = u512_test_vectors_512x512[i].a_mul_b_overflow;
      const tw_u512 expected = u512_test_vectors_512x512[i].a_mul_b;
      tw_u512 y = {0};
      tw_u512* y_ptr = &y;
      tw_u512 a = u512_test_vectors_512x512[i].a;
      tw_u512 b = u512_test_vectors_512x512[i].b;
      const tw_u512 a_old = a;
      const tw_u512 b_old = b;

      int overflow_out;
      char* name;
      if (j == 0) {
        name = "y = a * b";
        y_ptr = &y;
        overflow_out = tw_mul(&y, &a, &b);
      } else if (j == 1) {
        y_ptr = &a;
        overflow_out = tw_mul(&a, &a, &b);
        name = "a = a * b";
      } else if (j == 2) {
        y_ptr = &b;
        name = "b = a * b";
        overflow_out = tw_mul(&b, &a, &b);
      } else if (j == 3) {
        if (!tw_equal(&a, &b)) {
          continue;
        }
        y_ptr = &y;
        name = "y = a * a";
        overflow_out = tw_mul(&y, &a, &a);
      } else if (j == 4) {
        if (!tw_equal(&a, &b)) {
          continue;
        }
        y_ptr = &a;
        name = "a = a * a";
        overflow_out = tw_mul(&a, &a, &a);
      }

      ck_assert_msg(overflow_out == overflow, "Overflow check failed for %s check for vector %d", name, i);
      ck_assert_msg(tw_equal(y_ptr, &expected), "Multiply mismatch for %s check for vector %d", name, i);
      if (j != 1 && j != 4) {
        ck_assert_msg(tw_equal(&a_old, &a), "A altered for %s check for vector %d", name, i);
      }
      if (j != 2) {
        ck_assert_msg(tw_equal(&b_old, &b), "B altered for %s check for vector %d", name, i);
      }
    }
  }
}
END_TEST

/*
START_TEST (test_tw_div_rem) {
  tw_u512 a, b, y, z;
  for (int i = 0; i < U512_TEST_VECTORS_512X512_LENGTH; i++) {
    a = u512_test_vectors_512x512[i].a;
    b = u512_test_vectors_512x512[i].b;
    int div_by_0 = u512_test_vectors_512x512[i].div_by_0;
    ck_assert_msg(tw_div_rem(&y, &z, &a, &b) == div_by_0, "Divide by zero check failed for vector %d", i);
    if (!div_by_0) {
      ck_assert_msg(tw_equal(&y, &u512_test_vectors_512x512[i].a_div_b), "Division mismatch for vector %d", i);
      ck_assert_msg(tw_equal(&z, &u512_test_vectors_512x512[i].a_rem_b), "Remainder mismatch for vector %d", i);
    }
  }
}
END_TEST
*/

Suite * uint_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("UINT-512 Test Suite");

  /* Core test case */
  tc_core = tcase_create("Equal test");

  tcase_add_test(tc_core, test_tw_equal);
  tcase_add_test(tc_core, test_tw_compare);
  tcase_add_test(tc_core, test_tw_add);
  tcase_add_test(tc_core, test_tw_sub);
  tcase_add_test(tc_core, test_tw_mul);
  tcase_add_test(tc_core, test_tw_lshift);
  //tcase_add_test(tc_core, test_tw_div_rem);
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

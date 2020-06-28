/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"
#include "../src/math/src/tw_uint.h"
#include "vectors/vectors_u512.h"

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

Suite * uint_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("UINT-512 Test Suite");

  /* Core test case */
  tc_core = tcase_create("Equal test");

  tcase_add_test(tc_core, test_tw_equal);
  tcase_add_test(tc_core, test_tw_compare);
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

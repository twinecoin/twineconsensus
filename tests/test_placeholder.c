/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"

START_TEST (test_placeholder) {
    ck_assert_msg(1 == 1, "True check");
}
END_TEST

Suite * placeholder_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Placeholder Test Suite");

  /* Core test case */
  tc_core = tcase_create("Placeholder Test");

  tcase_add_test(tc_core, test_placeholder);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = placeholder_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

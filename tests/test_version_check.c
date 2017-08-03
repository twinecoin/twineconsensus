/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdlib.h>
#include <check.h>

#include "twineconsensus.h"

START_TEST (test_test_version) {
  ck_assert(twlib_test_version(0));
  ck_assert(!twlib_test_version(0xFFFFFFFF));
}
END_TEST

Suite * version_check_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Version Checks");

  /* Core test case */
  tc_core = tcase_create("Version 0 Check");

  tcase_add_test(tc_core, test_test_version);
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

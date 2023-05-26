#include "s21_test_decimal.h"

#include <check.h>

void tests_func_s21(Suite *s);

int main() {
  Suite *array_test_cases[20];

  array_test_cases[0] = suite_add();                    //
  array_test_cases[1] = suite_sub();                    //
  array_test_cases[2] = suite_div();                    //
  array_test_cases[3] = suite_mod();                    //
  array_test_cases[4] = suite_mul();                    //
  array_test_cases[5] = suite_from_decimal_to_float();  //
  array_test_cases[6] = suite_from_decimal_to_int();    //
  array_test_cases[7] = suite_from_float_to_decimal();  //
  array_test_cases[8] = suite_from_int_to_decimal();    //
  array_test_cases[9] = suite_is_equal();               //
  array_test_cases[10] = suite_is_not_equal();          //
  array_test_cases[11] = suite_is_greater();            //
  array_test_cases[12] = suite_is_greater_or_equal();   //
  array_test_cases[13] = suite_is_less();               //
  array_test_cases[14] = suite_is_less_or_equal();      //
  array_test_cases[15] = suite_truncate();              //
  array_test_cases[16] = suite_negate();                //
  array_test_cases[17] = suite_round();                 //
  array_test_cases[18] = suite_floor();                 //

  for (int i = 0; i < 19; i++) {
    tests_func_s21(array_test_cases[i]);
  }
  return 0;
}

void tests_func_s21(Suite *s) {
  SRunner *runner = srunner_create(s);
  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);
  srunner_free(runner);
}

/////////////////

// int main(void) {
//   run_tests();
//   return 0;
// }

// void run_testcase(Suite *testcase) {
//   static int counter_testcase = 1;
//   if (counter_testcase > 1) counter_testcase++;
//   SRunner *sr = srunner_create(testcase);
//   srunner_set_fork_status(sr, CK_NOFORK);
//   srunner_run_all(sr, CK_NORMAL);
//   srunner_free(sr);
// }

// void run_tests(void) {
//   Suite *list_cases[] = {suite_add(),
//                          suite_sub(),
//                          suite_div(),
//                          suite_mod(),
//                          suite_mul(),
//                          suite_from_decimal_to_float(),
//                          suite_from_decimal_to_int(),
//                          suite_from_float_to_decimal(),
//                          suite_from_int_to_decimal(),
//                          suite_is_equal(),
//                          suite_is_not_equal(),
//                          suite_is_greater(),
//                          suite_is_greater_or_equal(),
//                          suite_is_less(),
//                          suite_is_less_or_equal(),
//                          suite_truncate(),
//                          suite_negate(),
//                          suite_round(),
//                          suite_floor(),
//                          NULL};

//   for (Suite **current_testcase = list_cases; *current_testcase != NULL;
//        current_testcase++) {
//     run_testcase(*current_testcase);
//   }
// }

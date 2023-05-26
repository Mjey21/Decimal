#ifndef SRC_S21_TEST_DECIMAL_H_
#define SRC_S21_TEST_DECIMAL_H_

#include <check.h>
#include <stdio.h>
#include <wchar.h>

#include "s21_decimal.h"

#define RESET "\033[0m"
#define RED "\e[0;91m"
#define GREEN "\033[32m"
#define BLUE "\e[0;94m"

#define S21_TRUE 1
#define S21_FALSE 0
#define U_MAX_INT -1        // 0b11111111111111111111111111111111
#define MAX_INT 2147483647  // 0b01111111111111111111111111111111
#define MINUS -2147483648   // 0b10000000000000000000000000000000
#define MAX_DECIMAL 79228162514264337593543950335.0F
#define MIN_DECIMAL -79228162514264337593543950335.0F

Suite *suite_add();
Suite *suite_sub();
Suite *suite_div();
Suite *suite_mod();
Suite *suite_mul();
Suite *suite_from_decimal_to_float();
Suite *suite_from_decimal_to_int();
Suite *suite_from_float_to_decimal();
Suite *suite_from_int_to_decimal();
Suite *suite_is_equal();
Suite *suite_is_not_equal();
Suite *suite_is_greater();
Suite *suite_is_greater_or_equal();
Suite *suite_is_less();
Suite *suite_is_less_or_equal();
Suite *suite_truncate();
Suite *suite_negate();
Suite *suite_floor();
Suite *suite_round();

#endif  // SRC_S21_TEST_DECIMAL_H_
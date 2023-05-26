#include "../s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  s21_decimal_init(result);
  if (s21_is_zero(value_2)) {
    code = 3;
  } else {
    s21_decimal temp = {0};
    s21_div(value_1, value_2, &temp);
    s21_truncate(temp, &temp);
    s21_mul(temp, value_2, &temp);
    s21_sub(value_1, temp, result);
    s21_optimise(result);
  }
  return code;
}

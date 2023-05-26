#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  int p = 0;
  s21_decimal_init(result);
  s21_decimal near_zero = {{1, 0, 0, 0}};
  s21_set_exp(&near_zero, 28);
  s21_optimise(&value_1);
  s21_optimise(&value_2);
  if (s21_is_zero(value_2)) {
    code = 3;
  } else if (!s21_is_zero(value_1)) {
    p = value_1.bits[1] + value_1.bits[2] + value_2.bits[1] + value_2.bits[2];
    if (p == 0) {
      float res = (float)value_1.bits[0] / (float)value_2.bits[0];
      int n = 0;
      while (fmod(res, 1) != 0) {
        res *= 10;
        n++;
        if (n == 28) break;
      }
      s21_from_float_to_decimal(res, result);
      s21_set_exp(result, n);
    } else {
      big_decimal a, b, res;
      s21_big_init(&res);
      s21_to_big(value_1, &a);
      s21_to_big(value_2, &b);
      big_norm(&a, &b);
      big_div(a, b, &res);
      code = s21_from_big(res, result);
    }
  }
  p = s21_is_negative(value_1) + s21_is_negative(value_2);
  if (p == 1 && !s21_is_negative(*result)) s21_negate(*result, result);
  if (p == 1 && code == 1) code = 2;
  if (code == 0) s21_optimise(result);
  return code;
}

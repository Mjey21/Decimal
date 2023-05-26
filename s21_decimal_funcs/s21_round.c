#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int ret = !s21_is_valid(value);
  s21_decimal_init(result);
  s21_optimise(&value);
  int scale = s21_get_exp(value);
  if (!ret) {
    if (scale > 0) {
      s21_decimal five = {{5, 0, 0, 0}};
      s21_set_exp(&five, 1);
      if (s21_is_negative(value))
        s21_sub(value, five, result);
      else
        s21_add(value, five, result);
      ret = s21_truncate(*result, result);
    } else {
      *result = value;
    }
  }
  return ret;
}

#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  s21_decimal_init(result);
  s21_copy(value, result);
  s21_invert_bit(result, 127);
  return 0;
}

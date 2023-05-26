#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  float x = 0;
  int code = s21_from_decimal_to_float(src, &x);
  if (s21_get_exp(src) == 0 && (src.bits[1] != 0 || src.bits[2] != 0 ||
                              (src.bits[0] < 0 && src.bits[0] != -D))) {
    code = 1;
  } else if (s21_get_exp(src) == 0 && src.bits[0] >= 0) {
    *dst = src.bits[0];
    if (s21_is_negative(src)) *dst *= -1;
  } else if (s21_get_exp(src) == 0 && src.bits[0] == -D) {
    *dst = -0;
    if (s21_is_negative(src)) *dst *= -1;
  } else {
    x = trunc(x);
    if (x > INT_MAX || x < -D) {
      code = 1;
    } else {
      *dst = (int)x;
    }
  }
  return code;
}

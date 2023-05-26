#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  *dst = 0;
  double x = 0.0;
  double x1 = 0.0, x2 = 0.0, x3 = 0.0;
  int code = 0, scale = s21_get_exp(src);
  if (!s21_is_valid(src)) code = 1;
  if (code == 0) {
    x1 = s21_seg(src.bits[0]);
    x2 = LONG_MAX * s21_seg(src.bits[1]);
    x3 = LONG_LONG_MAX * s21_seg(src.bits[2]);
    x = x1 + x2 + x3;
    x /= pow(10, scale);
    if (s21_is_negative(src)) x *= -1;
  }
  *dst = (float)x;
  return code;
}

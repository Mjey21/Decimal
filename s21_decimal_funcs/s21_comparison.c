#include "../s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int code = 1;
  if ((s21_is_zero(a) ^ s21_is_zero(b))) return 0;
  if ((s21_is_zero(a) && s21_is_zero(b))) return 1;
  s21_optimise(&a);
  s21_optimise(&b);
  for (int i = 0; i < 4; i++)
    if (a.bits[i] != b.bits[i]) {
      code = 0;
      break;
    }
  return code;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !(s21_is_equal(a, b));
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  int code = -1;
  if (s21_is_negative(a) && !s21_is_negative(b)) code = 0;
  if (!s21_is_negative(a) && s21_is_negative(b)) code = 1;
  if (s21_is_equal(a, b)) code = 0;
  if (code == -1) {
    s21_optimise(&a);
    s21_optimise(&b);
    s21_normalisation(&a, &b);
    code = 1;
    for (int i = 95; i > -1; i--) {
      if (s21_get_bit(a, i) < s21_get_bit(b, i)) {
        code = 0;
        break;
      }
      if (s21_get_bit(a, i) > s21_get_bit(b, i)) {
        break;
      }
    }
    if (s21_is_negative(a) && s21_is_negative(b)) code = !code;
  }
  return code;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_equal(a, b) || s21_is_greater(a, b);
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  return !(s21_is_greater_or_equal(a, b));
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_equal(a, b) || s21_is_less(a, b);
}

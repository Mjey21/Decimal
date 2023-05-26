#include "../s21_decimal.h"

void left_shift_big(big_decimal *value);
int add_for_mul(big_decimal res_value, big_decimal value_1, big_decimal *res);
void right_shift(big_decimal *value);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  s21_decimal_init(result);
  big_decimal a = {0};
  big_decimal b = {0};
  big_decimal res = {0};
  s21_to_big(value_1, &a);
  if (a.n < 0) a.n = -a.n;
  s21_to_big(value_2, &b);
  if (b.n < 0) b.n = -b.n;
  big_mult(a, b, &res);
  code = s21_from_big(res, result);
  int p = s21_is_negative(value_1) + s21_is_negative(value_2);
  if (p == 1) s21_negate(*result, result);
  if (p == 1 && code == 1) code = 2;
  if (code == 0) s21_optimise(result);
  return code;
}

void big_mult(big_decimal a, big_decimal b, big_decimal *res) {
  int check = 0;
  int p = 0;
  if (b.bits[0] == 0 && b.bits[1] == 0 && b.bits[2] == 0 && b.bits[3] == 0 &&
      b.bits[4] == 0 && b.bits[5] == 0 && b.bits[6] == 0 && b.bits[7] == 0)
    p++;
  while (!p && !check) {
    if (get_bit(b.bits[0], 0)) check = add_for_mul(*res, a, res);
    left_shift_big(&a);
    if (get_bit(res->bits[5], 31)) check = 1;
    right_shift(&b);
    if (b.bits[0] == 0 && b.bits[1] == 0 && b.bits[2] == 0 && b.bits[3] == 0 &&
        b.bits[4] == 0 && b.bits[5] == 0 && b.bits[6] == 0 && b.bits[7] == 0)
      p++;
  }
  res->n = a.n + b.n;
  while (((res->bits[5] || res->bits[4] || res->bits[3]) && res->n != 0) ||
         res->n > 28) {
    s21_big_div_by_ten(res);
  }
}

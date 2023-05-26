#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  int sub = 0;
  int both_negative = 0;
  s21_decimal_init(result);
  if (s21_is_negative(value_1) && s21_is_negative(value_2)) {
    s21_negate(value_1, &value_1);
    s21_negate(value_2, &value_2);
    both_negative++;
  } else if (s21_is_negative(value_2) && !s21_is_negative(value_1)) {
    s21_negate(value_2, &value_2);
    code = s21_sub(value_1, value_2, result);
    sub++;
  } else if (s21_is_negative(value_1) && !s21_is_negative(value_2)) {
    s21_negate(value_1, &value_1);
    code = s21_sub(value_2, value_1, result);
    sub++;
  }

  if (!sub) {
    s21_decimal ed = {{5, 0, 0, 0}};
    s21_set_exp(&ed, 1);
    s21_decimal max = {{-1, -1, -1, 0}};
    if (s21_is_equal(value_1, max) && s21_is_less(value_2, ed)) {
      *result = value_1;
    } else if (s21_is_equal(value_2, max) && s21_is_less(value_1, ed)) {
      *result = value_2;
    } else if ((s21_is_equal(value_1, max) &&
                s21_is_greater_or_equal(value_2, ed)) ||
               (s21_is_equal(value_2, max) &&
                s21_is_greater_or_equal(value_1, ed))) {
      code = 1;
    } else {
      int norm = s21_normalisation(&value_1, &value_2);
      if (norm >= 0) {
        int flag = s21_simple_add(value_1, value_2, result);
        code = s21_check_result(result, flag);
        s21_set_exp(result, norm);
      }
      if (norm < 0 || code != 0) {
        code = s21_big_add(value_1, value_2, result);
      }
    }
    if (code == 0) s21_optimise(result);
    if (both_negative && !s21_is_negative(*result)) s21_negate(*result, result);
    if (both_negative && code == 1) code = 2;
  }
  return code;
}

int s21_simple_add(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  int flag = 0;
  for (int i = 0; i < 96; i++) {
    int add = s21_get_bit(value_1, i) + s21_get_bit(value_2, i) + flag;
    flag = 0;
    if (add == 1) {
      s21_set_bit(result, i, 1);
    } else if (add == 2) {
      flag = 1;
    } else if (add == 3) {
      s21_set_bit(result, i, 1);
      flag = 1;
    }
  }
  return flag;
}

int s21_check_result(s21_decimal *result, int flag) {
  int code = 0;
  if (flag == 1 && !s21_is_negative(*result)) {
    code = 1;
  }
  if (flag == 1 && s21_is_negative(*result)) {
    code = 2;
  }
  return code;
}

int s21_big_add(s21_decimal src1, s21_decimal src2, s21_decimal *res) {
  int ret = 0;
  big_decimal a, b, result = {{0}, 0};
  s21_decimal_init(res);
  s21_to_big(src1, &a);
  s21_to_big(src2, &b);
  s21_big_init(&result);
  ret = big_plus(a, b, &result);
  if (!ret)
    ret = s21_from_big(result, res);
  else
    s21_decimal_init(res);
  return ret;
}

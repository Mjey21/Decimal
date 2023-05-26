#include "../s21_decimal.h"

int float_sign(int *i, char my_float[20]);
int get_num(int *i, int *float_pow, char my_float[20]);
int get_true_pow(int *i, int *pow_sign, int *float_pow, char my_float[20]);
int s21_drop_last_digit(s21_decimal *src);
int true_converter(int num, int float_pow, int pow_sign, s21_decimal *dst,
                   float src);

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  char my_float[20] = {'\0'};
  sprintf(my_float, "%e", src);
  s21_decimal_init(dst);
  int ret = 0;
  int sign = 0;
  int float_pow = 1;
  int pow_sign = 0;
  int num = 0;
  int i = 0;
  int true_pow = 0;
  if (isnan(src) || isinf(src)) {
    ret = 1;
  } else {
    sign = float_sign(&i, my_float);
    num = get_num(&i, &float_pow, my_float);
    true_pow = get_true_pow(&i, &pow_sign, &float_pow, my_float);
    float_pow += true_pow;
    if (i) i = 0;
    if (true_pow < 29 && true_pow > -29) {
      ret = true_converter(num, float_pow, pow_sign, dst, src);
    } else {
      ret = 1;
    }
    if (sign) s21_negate(*dst, dst);
    if (ret) {
      ret = 1;
      s21_decimal_init(dst);
    }
  }
  return ret;
}

int float_sign(int *i, char my_float[20]) {
  int sign = 0;
  if (my_float[*i] == '-') {
    sign = 1;
    *i += 1;
  }
  return sign;
}

int get_num(int *i, int *float_pow, char my_float[20]) {
  int num = 0;
  for (; my_float[*i] != 'e'; *i += 1) {
    if (my_float[*i] == '.') continue;
    num *= 10;
    num += my_float[*i] - '0';
    *float_pow -= 1;
  }
  *i += 1;
  return num;
}

int get_true_pow(int *i, int *pow_sign, int *float_pow, char my_float[20]) {
  int tmp = 0;
  if (my_float[*i] == '-') {
    *pow_sign = 1;
    *i += 1;
  } else if (my_float[*i] == '+') {
    *i += 1;
  }
  for (; my_float[*i]; *i += 1) {
    tmp *= 10;
    tmp += my_float[*i] - '0';
  }
  if (*pow_sign) *float_pow *= -1;
  return tmp;
}

int true_converter(int num, int float_pow, int pow_sign, s21_decimal *dst,
                   float src) {
  int ret = 0;
  while (num % 10 == 0 && float_pow != 0) {
    num /= 10;
    float_pow += pow_sign ? -1 : 1;
  }
  s21_decimal_init(dst);
  dst->bits[0] = num;
  if ((!pow_sign && float_pow > 0)) {
    while (float_pow) {
      s21_decimal dec = {{10, 0, 0, 0}};
      ret = s21_mul(*dst, dec, dst);
      float_pow--;
      if (ret) break;
    }
    s21_set_exp(dst, abs(float_pow));
  } else {
    int last = 0;
    if (float_pow > 28) {
      char float_tmp[20] = {'\0'};
      sprintf(float_tmp, "%.7e", src);
      if (float_tmp[2] == '.') {
        last = float_tmp[3] - '0';
      } else {
        last = float_tmp[2] - '0';
      }
    }
    while (float_pow > 28) {
      s21_drop_last_digit(dst);
      float_pow--;
    }
    if (last > 4) dst->bits[0] += 1;
    s21_set_exp(dst, abs(float_pow));
  }
  return ret;
}

int s21_drop_last_digit(s21_decimal *src) {
  unsigned long long last, div;

  last = (unsigned int)(src->bits[2]);
  div = last % 10;
  src->bits[2] = (int)((unsigned int)(last / 10));

  last = (unsigned int)(src->bits[1]) + div * 2 * D;
  div = last % 10;
  src->bits[1] = (int)((unsigned int)(last / 10));

  last = (unsigned int)(src->bits[0]) + div * 2 * D;
  div = last % 10;
  src->bits[0] = (int)((unsigned int)(last / 10));
  return (int)div;
}

#include "../s21_decimal.h"

int s21_get_digit(int index) { return index >> 5; }

int s21_get_index(int index) { return index % 32; }

int s21_get_bit(s21_decimal number, int index) {
  int i = s21_get_digit(index);
  int ind = s21_get_index(index);
  return (number.bits[i] >> ind) & 1;
}

void s21_invert_bit(s21_decimal *number, int index) {
  int i = s21_get_digit(index);
  int ind = s21_get_index(index);
  if (i >= 0 && i <= 3) number->bits[i] ^= 1 << ind;
}

void s21_set_bit(s21_decimal *number, int index, int bit) {
  int i = s21_get_digit(index);
  int ind = s21_get_index(index);
  int mask = 1 << ind;
  if (number) {
    number->bits[i] &= ~mask;
    if (bit) {
      number->bits[i] |= mask;
    }
  }
}

int s21_is_negative(s21_decimal number) { return s21_get_bit(number, 127); }

void s21_copy(s21_decimal number, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = number.bits[i];
  }
}

int s21_get_exp(s21_decimal number) {
  int scale = 1, result = 0;
  for (int i = 112; i < 117; i++) {
    if (s21_get_bit(number, i)) {
      result += scale;
    }
    scale <<= 1;
  }
  return result;
}

void s21_decimal_init(s21_decimal *result) {
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;
}

void s21_set_exp(s21_decimal *value, int scale) {
  int scale_index = 112;
  if (scale > 28) scale = 28;
  if (s21_is_negative(*value))
    value->bits[3] = 0b10000000000000000000000000000000;
  else
    value->bits[3] = 0;

  while (scale != 0) {
    if ((scale & 0x1) == 0x1) s21_set_bit(value, scale_index, 1);
    scale >>= 1;
    scale_index++;
  }
}

void s21_div_by_ten(s21_decimal value, s21_decimal *result) {
  result->bits[3] = value.bits[3];
  result->bits[2] = (int)(value.bits[2] / 10);
  int x = 10 * (value.bits[2] - result->bits[2]);
  result->bits[1] = (int)((value.bits[1] + x * INT_MAX) / 10);
  x = 10 * (value.bits[1] - result->bits[1]);
  result->bits[0] = (int)((value.bits[0] + x * INT_MAX) / 10);
  x = (int)fmod(value.bits[0], 10);
}

int s21_is_zero(s21_decimal value) {
  int code = 1;
  for (int row = 0; row < 3; row++) {
    if (value.bits[row] != 0) {
      code = 0;
      break;
    }
  }
  return code;
}

void s21_shift_left(s21_decimal *value) {
  for (int i = 0; i < 96; i++) {
    s21_set_bit(value, i, s21_get_bit(*value, i + 1));
  }
}

int s21_is_valid(s21_decimal value) {
  int code = 0;
  if ((value.bits[3] & 0b01111111000000001111111111111111) == 0) code = 1;
  return code;
}

long int s21_seg(int a) {
  long int ret = a;
  if (a < 0) ret = a + 2 * D;
  return ret;
}

int s21_dec(float a) {
  if (a >= D) a = -2 * D + a;
  return (int)a;
}

int s21_mul_by_ten(s21_decimal value, s21_decimal *result) {
  int ret = 0;
  int scale = s21_get_exp(value);
  float x = s21_seg(value.bits[0]) + LONG_MAX * s21_seg(value.bits[1]) +
            LONG_LONG_MAX * s21_seg(value.bits[2]);
  x = x * 10;
  scale = scale - 1;
  if (scale < 0) scale = 0;
  if (fabs(x / pow(10, scale)) > S21_MAX) {
    ret++;
  } else {
    if (x >= LONG_LONG_MAX) {
      result->bits[2] = s21_dec(trunc(x / LONG_LONG_MAX));
      result->bits[1] =
          s21_dec(trunc((x - trunc(x / LONG_LONG_MAX) / LONG_MAX)));
      result->bits[0] =
          s21_dec(x - trunc((x - trunc(x / LONG_LONG_MAX) / LONG_MAX)));
    } else if (x >= LONG_MAX) {
      result->bits[0] = s21_dec(x - trunc(x / LONG_MAX));
      result->bits[1] = s21_dec(trunc(x / LONG_MAX));
    } else {
      result->bits[0] = s21_dec(x);
    }
    result->bits[3] = 0;
    s21_set_exp(result, scale);
    if (s21_is_negative(value)) s21_negate(*result, result);
  }
  return ret;
}

int s21_normalisation(s21_decimal *value1, s21_decimal *value2) {
  int n1 = s21_get_exp(*value1);
  int n2 = s21_get_exp(*value2);
  s21_decimal a = {{0}}, b = {{0}};
  s21_copy(*value1, &a);
  s21_copy(*value2, &b);
  int n3 = n1 > n2 ? n1 : n2;
  int p = 0;
  if (n1 > n2) {
    for (int i = 0; i < n1 - n2; i++) p += s21_mul_by_ten(b, &b);
  } else {
    for (int i = 0; i < n2 - n1; i++) p += s21_mul_by_ten(a, &a);
  }
  if (p != 0) {
    n3 = -1;
  } else {
    s21_copy(a, value1);
    s21_copy(b, value2);
    s21_set_exp(value1, n3);
    s21_set_exp(value2, n3);
  }
  return n3;
}

void s21_optimise(s21_decimal *val) {
  int n = s21_get_exp(*val);
  if (n != 0 && val->bits[0] % 10 == 0) {
    while (n) {
      if (val->bits[0] % 10 == 0) {
        s21_div_by_ten(*val, val);
        n--;
      } else {
        break;
      }
    }
  }
  s21_set_exp(val, n);
}

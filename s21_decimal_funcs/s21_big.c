#include "../s21_decimal.h"

int get_bit(int src, int pos) {
  if (src & (1 << pos)) {
    return 1;
  } else {
    return 0;
  }
}

void set_bit(int *dst, int pos, int value) {
  if (value) {
    *dst |= (1 << pos);
  } else {
    *dst &= ~(1 << pos);
  }
}

void s21_big_div_by_ten(big_decimal *src) {
  unsigned long long last, div;

  div = 0;
  for (int i = 5; i >= 0; i--) {
    last = (unsigned int)(src->bits[i]) + div * 2 * D;
    div = last % 10;
    src->bits[i] = (int)((unsigned int)(last / 10));
  }
  --src->n;
}

int s21_big_get_bit(big_decimal d, int i) {
  int result = 0;
  int mask = 1;
  int val = i / 32;
  if (d.bits[val] & (mask << (i - (32 * val)))) {
    result = 1;
  }
  return result;
}

void s21_big_turn_bit_on(big_decimal *d, int i) {
  int mask = 1;
  int val = i / 32;
  d->bits[val] = d->bits[val] | (mask << (i - (32 * val)));
}

void s21_big_turn_bit_off(big_decimal *d, int i) {
  int mask = 1;
  int val = i / 32;
  d->bits[val] = d->bits[val] & (~(mask << (i - (32 * val))));
}

void s21_big_shift(big_decimal *a, int flag) {
  if (flag == 1) {
    for (int i = 255; i != 0; i--) {
      if (s21_big_get_bit(*a, i - 1)) {
        s21_big_turn_bit_on(a, i);
      } else {
        s21_big_turn_bit_off(a, i);
      }
    }
    s21_big_turn_bit_off(a, 0);
  } else if (flag == -1) {
    for (int i = 0; i < 256; i++) {
      if (s21_big_get_bit(*a, i + 1)) {
        s21_big_turn_bit_on(a, i);
      } else {
        s21_big_turn_bit_off(a, i);
      }
    }
    s21_big_turn_bit_off(a, 255);
  }
}

void s21_to_big(s21_decimal a, big_decimal *b) {
  b->bits[0] = a.bits[0];
  b->bits[1] = a.bits[1];
  b->bits[2] = a.bits[2];
  b->bits[3] = 0;
  b->bits[4] = 0;
  b->bits[5] = 0;
  b->bits[6] = 0;
  b->bits[7] = 0;
  b->n = s21_get_exp(a);
  if (s21_is_negative(a)) b->n *= -1;
}

void s21_big_init(big_decimal *a) {
  a->bits[0] = 0;
  a->bits[1] = 0;
  a->bits[2] = 0;
  a->bits[3] = 0;
  a->bits[4] = 0;
  a->bits[5] = 0;
  a->bits[6] = 0;
  a->bits[7] = 0;
  a->n = 0;
}

int s21_from_big(big_decimal a, s21_decimal *b) {
  int ret = 0;
  int scale = abs(a.n);
  int sign = 0;
  if (a.n < 0) sign++;
  s21_decimal_init(b);
  if (scale <= 28 && a.bits[3] == 0 && a.bits[4] == 0 && a.bits[5] == 0 &&
      a.bits[6] == 0 && a.bits[7] == 0) {
    b->bits[0] = a.bits[0];
    b->bits[1] = a.bits[1];
    b->bits[2] = a.bits[2];
    b->bits[3] = 0;
    s21_set_exp(b, abs(a.n));
    if (sign) s21_negate(*b, b);
    for (int i = 0; i < 3; i++)
      if (b->bits[i] < 0) b->bits[i] = -1;
  } else {
    ret = 1;
  }
  return ret;
}

int big_norm(big_decimal *a, big_decimal *b) {
  int n1 = abs(a->n);
  int n2 = abs(b->n);
  int n3 = n1 > n2 ? n1 : n2;
  if (n1 > n2) {
    for (int i = 0; i < n1 - n2; i++) big_mul_by_ten(*b, b);
  } else {
    for (int i = 0; i < n2 - n1; i++) big_mul_by_ten(*a, a);
  }
  return n3;
}

void big_mul_by_ten(big_decimal a, big_decimal *b) {
  s21_big_init(b);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int check = 0;
  while (value_2.bits[0] != 0 && !check) {
    if (get_bit(value_2.bits[0], 0)) check = add_for_mul(*b, a, b);
    left_shift_big(&a);
    if (get_bit(b->bits[5], 31)) check = 1;
    s21_shift_left(&value_2);
  }
  b->n = a.n;
}

int big_plus(big_decimal a, big_decimal b, big_decimal *res) {
  int ret = 0;
  res->n = big_norm(&a, &b);
  int add_bit = 0;
  for (int i = 0; i < 192; i++) {
    int bita = get_bit(a.bits[i / 32], i % 32);
    int bitb = get_bit(b.bits[i / 32], i % 32);
    set_bit(&(res->bits[i / 32]), i % 32, bita ^ bitb ^ add_bit);
    add_bit = (bita && bitb) || (bitb && add_bit) || (bita && add_bit);
  }

  while ((res->bits[7] || res->bits[6] || res->bits[5] || res->bits[4] ||
          res->bits[3]) &&
         res->n > 0) {
    s21_big_div_by_ten(res);
  }
  return ret;
}

void big_minus(big_decimal a, big_decimal b, big_decimal *res) {
  s21_big_init(res);
  res->n = big_norm(&a, &b);
  big_invert(a, &a);
  big_plus(a, b, res);
  big_invert(*res, res);
}

void big_invert(big_decimal b, big_decimal *d) {
  big_decimal tmp = {0};
  tmp.bits[0] = ~(b.bits[0]);
  tmp.bits[1] = ~(b.bits[1]);
  tmp.bits[2] = ~(b.bits[2]);
  tmp.bits[3] = ~(b.bits[3]);
  tmp.bits[4] = ~(b.bits[4]);
  tmp.bits[5] = ~(b.bits[5]);
  tmp.bits[6] = ~(b.bits[6]);
  tmp.bits[7] = ~(b.bits[7]);
  big_decimal plus_one = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
  big_plus(plus_one, tmp, d);
}

void big_div(big_decimal a, big_decimal b, big_decimal *result) {
  int count_a = s21_big_get_count(a);
  int count_b = s21_big_get_count(b);
  if (count_a < count_b) return;

  int step_count = count_a - count_b;
  for (int i = 0; i < count_a - count_b; i++) {
    s21_big_shift(&b, 1);
  }

  for (int hz = 0; hz != step_count + 1; hz++) {
    if (!can_contain(a, b)) {
      s21_big_shift(&b, -1);
      if (hz != 0) {
        s21_big_shift(result, 1);
      }
    } else {
      big_decimal tmp_result = {0};
      big_minus(a, b, &tmp_result);
      s21_big_shift(result, 1);
      s21_big_turn_bit_on(result, 0);
      s21_big_shift(&b, -1);
      a = tmp_result;
    }
  }
}

int s21_big_get_count(big_decimal a) {
  int count = 0;
  for (int i = 255; i > 0; i--) {
    if (s21_big_get_bit(a, i)) {
      count = i;
      break;
    }
  }
  return count;
}

int can_contain(big_decimal a, big_decimal b) {
  int result = 1;
  for (int i = 255; i != -1; i--) {
    if (s21_big_get_bit(a, i) != s21_big_get_bit(b, i)) {
      if (s21_big_get_bit(a, i) == 0) {
        result = 0;
      }
      break;
    }
  }
  return result;
}

void left_shift_big(big_decimal *value) {
  int old = 0;
  for (int i = 0; i < 6; i++) {
    int new_bit = get_bit(value->bits[i], 31);
    value->bits[i] = value->bits[i] << 1;
    set_bit(&(value->bits[i]), 0, old);
    old = new_bit;
  }
}

int add_for_mul(big_decimal res_value, big_decimal value_1, big_decimal *res) {
  int check = 0;
  int add_bit = 0;
  for (int i = 0; i < 192; i++) {
    int a = get_bit(value_1.bits[i / 32], i % 32);
    int b = get_bit(res_value.bits[i / 32], i % 32);
    set_bit(&(res->bits[i / 32]), i % 32, a ^ b ^ add_bit);
    add_bit = (a && b) || (b && add_bit) || (a && add_bit);
  }
  if (add_bit) check = 1;
  return check;
}

void right_shift(big_decimal *value) {
  int old = 0;
  for (int i = 7; i >= 0; i--) {
    int new_bit = get_bit(value->bits[i], 0);
    value->bits[i] = value->bits[i] >> 1;
    set_bit(&(value->bits[i]), 31, old);
    old = new_bit;
  }
}

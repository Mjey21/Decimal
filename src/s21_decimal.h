#ifndef _SRC_S21_DECIMAL_H_
#define _SRC_S21_DECIMAL_H_

#include <stdio.h>

typedef struct {
    int bits[4];
} s21_decimal;

typedef struct {
  unsigned bits[7];
} s21_big_decimal;

int s21_get_bit_big(s21_big_decimal src, int index);
void s21_set_bit_big(s21_big_decimal *dst, int index, int bit);

int s21_get_bit(s21_decimal src, int index);
int s21_get_scale(s21_decimal dst);
int s21_get_sign(s21_decimal dst);

void s21_set_bit(s21_decimal *dst, int index, int bit);
void s21_set_scale(s21_decimal *dst, int scale);
void s21_set_sign(s21_decimal *dst);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);


#endif  // _SRC_S21_DECIMAL_H_

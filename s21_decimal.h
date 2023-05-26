#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define min 1e-28
#define INT_MAX 2147483647
#define LONG_MAX 4294967296.0
#define LONG_LONG_MAX 18446744073709552000.0  // 1.8446744e+19
#define D 2147483648.0
#define inf 1.0 / 0.0
#define maxix -1
#define S21_MAX 79228162514264337593543950335.0

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[8];
  int n;
} big_decimal;

// s21_utils.c

// Получить индекс слова по общему индексу бита
int s21_get_digit(int index);
// Получить индекс бита внутри слова по общему индексу
int s21_get_index(int index);
// Получить значения бита по общему индексу
int s21_get_bit(s21_decimal number, int index);
// Получить степень делителя number
int s21_get_exp(s21_decimal number);
// Заменить значение бита по общему индексу на значение bit
void s21_set_bit(s21_decimal *number, int index, int bit);
// Записать scale в степень делителя number
void s21_set_exp(s21_decimal *number, int exp);
// Инвертировать бит по общему индексу
void s21_invert_bit(s21_decimal *number, int index);
// Отрицателен ли number?
int s21_is_negative(s21_decimal number);
// Равен ли value +0?
int s21_is_zero(s21_decimal value);
// Занулены ли запрещенные биты в value.bits[3]?
int s21_is_valid(s21_decimal value);
// Обнулить result
void s21_decimal_init(s21_decimal *result);
// Побитовый сдвиг влево
void s21_shift_left(s21_decimal *value);
// Скопировать number в result
void s21_copy(s21_decimal number, s21_decimal *result);
// Преобразовать слово a в формат long int
long int s21_seg(int a);
// Преобразовать float a в формат слова
int s21_dec(float a);
// Разделить делимую часть value на 10, сохранить в result
void s21_div_by_ten(s21_decimal value, s21_decimal *result);
// Умножить делимую часть value на 10, увеличить степень делителя на 1,
// сохранить в result.
// Возвращаемое значение:
// Успех - 0; Переполнение - 1
int s21_mul_by_ten(s21_decimal value, s21_decimal *result);
// Сравнять степени делителя value1 и value2 в большую сторону
// Возвращаемое значение:
// Успех - 0; Переполнение - 1
int s21_normalisation(s21_decimal *value1, s21_decimal *value2);
// Перенести степени деления возможные без остатка в делимую часть
void s21_optimise(s21_decimal *val);

int s21_simple_add(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result);
int s21_simple_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res);
int s21_check_result(s21_decimal *result, int flag);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_add(s21_decimal src1, s21_decimal src2, s21_decimal *res);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_big_sub(s21_decimal src1, s21_decimal src2, s21_decimal *res);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// s21_big.c

// Заменить значение бита в слове dst по индексу pos на value
void set_bit(int *dst, int pos, int value);
// Получить значение бита в слове src  по индексу pos
int get_bit(int src, int pos);
// Разделить делимую часть src на 10
void s21_big_div_by_ten(big_decimal *src);
// Получить значения бита по общему индексу
int s21_big_get_bit(big_decimal d, int i);
// Включить бит по общему индексу
void s21_big_turn_bit_on(big_decimal *d, int i);
// Выключить бит по общему индексу
void s21_big_turn_bit_off(big_decimal *d, int i);
// При flag= 1 сдвиг вправо
// При flag=-1 сдвиг влево
void s21_big_shift(big_decimal *a, int flag);
// Обнулить a
void s21_big_init(big_decimal *a);
// Преобразовать s21_decimal в big_decimal
void s21_to_big(s21_decimal a, big_decimal *b);
// Преобразовать big_decimal в s21_decimal
int s21_from_big(big_decimal a, s21_decimal *b);
// Понизить степень делителя bd на val, за счет деления делимой части
void s21_big_dec_exp(big_decimal *bd, int val, big_decimal *result);
// Получить индекс последнего включенного бита
int s21_big_get_count(big_decimal a);
// Умножить делимую часть на 10, не меняет степень делителя
void big_mul_by_ten(big_decimal a, big_decimal *b);
// Сравнять степени делителя a и b в большую сторону
int big_norm(big_decimal *a, big_decimal *b);

int big_plus(big_decimal a, big_decimal b, big_decimal *result);
void big_minus(big_decimal a, big_decimal b, big_decimal *res);
void big_mult(big_decimal a, big_decimal b, big_decimal *result);
void big_div(big_decimal a, big_decimal b, big_decimal *result);
// Инвертировать все биты b, записать в d
void big_invert(big_decimal b, big_decimal *d);
// Использует ли a больше или столько же битов чем b
int can_contain(big_decimal a, big_decimal b);

void left_shift_big(big_decimal *value);
int add_for_mul(big_decimal res_value, big_decimal value_1, big_decimal *res);
void right_shift(big_decimal *value);

#endif  // S21_DECIMAL_H_

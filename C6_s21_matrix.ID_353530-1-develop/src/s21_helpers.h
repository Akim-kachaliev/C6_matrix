#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include "s21_matrix.h"

int s21_is_correct_matrix(const s21_matrix *A);
int s21_same_size(const s21_matrix *A, const s21_matrix *B);
int s21_is_square(const s21_matrix *A);
void s21_copy_matrix(const s21_matrix *src, s21_matrix *dst);
int s21_create_minor(const s21_matrix *A, int row, int column,
                     s21_matrix *minor);

extern int test_memory_fail_flag;
extern int test_det_fail_flag;
extern int test_create_matrix_fail_flag;
extern int test_create_matrix_row_fail_flag;
extern int test_transpose_fail_flag;

#endif
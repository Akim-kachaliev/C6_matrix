#include "s21_helpers.h"

int test_transpose_fail_flag = 0;

int s21_transpose(s21_matrix *A, s21_matrix *result) {
  if (test_transpose_fail_flag) {
    test_transpose_fail_flag = 0;
    return INCORRECT_MATRIX;
  }

  if (!s21_is_correct_matrix(A) || result == NULL) return INCORRECT_MATRIX;

  int status = s21_create_matrix(A->columns, A->rows, result);
  if (status != OK) return status;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }

  return OK;
}
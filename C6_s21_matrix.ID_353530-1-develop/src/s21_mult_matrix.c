#include "s21_helpers.h"

int s21_mult_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B) || result == NULL)
    return INCORRECT_MATRIX;

  if (A->columns != B->rows) return CALC_ERROR;

  int status = s21_create_matrix(A->rows, B->columns, result);
  if (status != OK) return status;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = 0.0;
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }

  return OK;
}
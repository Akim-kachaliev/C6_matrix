#include "s21_helpers.h"

int s21_sub_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B) || result == NULL)
    return INCORRECT_MATRIX;

  if (!s21_same_size(A, B)) return CALC_ERROR;

  int status = s21_create_matrix(A->rows, A->columns, result);
  if (status != OK) return status;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }

  return OK;
}
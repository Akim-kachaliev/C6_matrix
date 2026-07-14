#include "s21_helpers.h"
#include "s21_matrix.h"

int s21_mult_number(s21_matrix *A, double number, s21_matrix *result) {
  if (!s21_is_correct_matrix(A) || result == NULL) {
    return INCORRECT_MATRIX;
  }

  int status = s21_create_matrix(A->rows, A->columns, result);
  if (status != OK) {
    return status;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return OK;
}
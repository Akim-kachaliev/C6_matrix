#include "s21_helpers.h"
#include "s21_matrix.h"

int s21_calc_complements(const s21_matrix *A, s21_matrix *result) {
  if (!s21_is_correct_matrix(A) || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (!s21_is_square(A)) return CALC_ERROR;

  int status = s21_create_matrix(A->rows, A->columns, result);
  if (status != OK) return status;

  if (A->rows == 1) {
    result->matrix[0][0] = 1.0;
    return OK;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      s21_matrix minor;
      double det = 0.0;

      status = s21_create_minor(A, i, j, &minor);
      if (status != OK) {
        s21_remove_matrix(result);
        return status;
      }

      status = s21_determinant(&minor, &det);
      if (status != OK) {
        s21_remove_matrix(&minor);
        s21_remove_matrix(result);
        return status;
      }

      result->matrix[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * det;
      s21_remove_matrix(&minor);
    }
  }

  return OK;
}
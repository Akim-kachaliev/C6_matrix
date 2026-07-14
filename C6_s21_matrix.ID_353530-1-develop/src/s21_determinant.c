#include "s21_helpers.h"

int s21_determinant(s21_matrix *A, double *result) {
  if (test_det_fail_flag) {
    test_det_fail_flag = 0;
    return INCORRECT_MATRIX;
  }

  if (A == NULL || A->matrix == NULL || result == NULL || A->rows <= 0 ||
      A->columns <= 0)
    return INCORRECT_MATRIX;

  if (!s21_is_square(A)) return CALC_ERROR;

  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return OK;
  }

  if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    return OK;
  }

  *result = 0;
  int status = OK;

  for (int j = 0; j < A->columns && status == OK; j++) {
    s21_matrix minor;

    status = s21_create_minor(A, 0, j, &minor);
    if (status != OK) return status;

    double det = 0;
    status = s21_determinant(&minor, &det);

    if (status == OK) {
      if (j % 2 == 0)
        *result += A->matrix[0][j] * det;
      else
        *result -= A->matrix[0][j] * det;
    }

    s21_remove_matrix(&minor);
  }

  return status;
}
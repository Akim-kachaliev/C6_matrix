#include "s21_helpers.h"

int s21_inverse_matrix(s21_matrix *A, s21_matrix *result) {
  if (!s21_is_correct_matrix(A) || result == NULL) return INCORRECT_MATRIX;

  if (!s21_is_square(A)) return CALC_ERROR;

  double det = 0.0;

  int status = s21_determinant(A, &det);

  if (status != OK) return status;

  if (fabs(det) < 1e-7) return CALC_ERROR;

  s21_matrix complements;
  s21_matrix transpose;

  status = s21_calc_complements(A, &complements);

  if (status != OK) {
    return status;
  }

  status = s21_transpose(&complements, &transpose);

  s21_remove_matrix(&complements);

  if (status != OK) {
    return status;
  }

  status = s21_mult_number(&transpose, 1.0 / det, result);

  s21_remove_matrix(&transpose);

  return status;
}
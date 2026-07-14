#include <math.h>

#include "s21_helpers.h"
#include "s21_matrix.h"

int s21_eq_matrix(const s21_matrix *A, const s21_matrix *B) {
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B)) {
    return FAILURE;
  }

  if (!s21_same_size(A, B)) {
    return FAILURE;
  }

  int result = SUCCESS;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      // Проверяем на NaN: если хотя бы одно значение NaN, матрицы не равны
      if (isnan(A->matrix[i][j]) || isnan(B->matrix[i][j]) ||
          fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
        result = FAILURE;
        break;
      }
    }
  }
  return result;
}
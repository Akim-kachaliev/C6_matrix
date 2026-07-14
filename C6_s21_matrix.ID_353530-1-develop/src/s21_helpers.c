#include "s21_helpers.h"

int test_memory_fail_flag = 0;
int test_det_fail_flag = 0;

int s21_is_correct_matrix(const s21_matrix *A) {
  return (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0);
}

int s21_same_size(const s21_matrix *A, const s21_matrix *B) {
  return (A != NULL && B != NULL && A->rows == B->rows &&
          A->columns == B->columns);
}

int s21_is_square(const s21_matrix *A) {
  return (A != NULL && A->rows == A->columns);
}

void s21_copy_matrix(const s21_matrix *src, s21_matrix *dst) {
  if (src != NULL && dst != NULL) {
    for (int i = 0; i < src->rows; i++) {
      for (int j = 0; j < src->columns; j++) {
        dst->matrix[i][j] = src->matrix[i][j];
      }
    }
  }
}

int s21_create_minor(const s21_matrix *A, int row, int column,
                     s21_matrix *minor) {
  if (test_memory_fail_flag) {
    test_memory_fail_flag = 0;
    return INCORRECT_MATRIX;
  }

  if (!s21_is_correct_matrix(A) || minor == NULL) {
    return INCORRECT_MATRIX;
  }

  int status = s21_create_matrix(A->rows - 1, A->columns - 1, minor);
  if (status == OK) {
    int m_i = 0;
    for (int i = 0; i < A->rows; i++) {
      if (i == row) continue;
      int m_j = 0;
      for (int j = 0; j < A->columns; j++) {
        if (j == column) continue;
        minor->matrix[m_i][m_j] = A->matrix[i][j];
        m_j++;
      }
      m_i++;
    }
  }
  return status;
}
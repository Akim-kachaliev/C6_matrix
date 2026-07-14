#include "s21_matrix.h"

int test_create_matrix_fail_flag = 0;
int test_create_matrix_row_fail_flag = 0;

int s21_create_matrix(int rows, int columns, s21_matrix *result) {
  if (test_create_matrix_fail_flag) {
    test_create_matrix_fail_flag = 0;
    return INCORRECT_MATRIX;
  }

  if (rows <= 0 || columns <= 0 || result == NULL) {
    return INCORRECT_MATRIX;
  }

  int status = OK;
  result->rows = rows;
  result->columns = columns;

  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix == NULL) {
    status = INCORRECT_MATRIX;
  } else {
    for (int i = 0; i < rows; i++) {
      if (test_create_matrix_row_fail_flag && i == 1) {
        test_create_matrix_row_fail_flag = 0;
        result->matrix[i] = NULL;
      } else {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
      }

      if (result->matrix[i] == NULL) {
        for (int j = 0; j < i; j++) {
          free(result->matrix[j]);
        }
        free(result->matrix);
        result->matrix = NULL;
        status = INCORRECT_MATRIX;
        break;
      }
    }
  }

  return status;
}
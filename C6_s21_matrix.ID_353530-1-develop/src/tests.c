#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_helpers.h"
#include "s21_matrix.h"

extern int test_memory_fail_flag;
extern int test_det_fail_flag;
extern int test_create_matrix_fail_flag;
extern int test_create_matrix_row_fail_flag;
extern int test_transpose_fail_flag;

// Тесты: create и remove matrix
START_TEST(create_matrix_ok) {
  s21_matrix A;
  int status = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(status, OK);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 3);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_matrix_fail) {
  s21_matrix A;
  ck_assert_int_eq(s21_create_matrix(-1, 5, &A), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(5, -1, &A), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(0, 5, &A), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(5, 0, &A), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(-1, -1, &A), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(0, 0, &A), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_create_matrix(3, 3, NULL), INCORRECT_MATRIX);
}
END_TEST

START_TEST(create_matrix_row_alloc_fail) {
  s21_matrix A;
  test_create_matrix_row_fail_flag = 1;
  int status = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
}
END_TEST

// Тесты: equal matrix
START_TEST(eq_matrix_true) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0000001;
  B.matrix[0][0] = 1.0000002;
  A.matrix[0][1] = 2.5;
  B.matrix[0][1] = 2.5;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_false) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&B);

  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 1.00001;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  ck_assert_int_eq(s21_eq_matrix(NULL, &B), FAILURE);
  ck_assert_int_eq(s21_eq_matrix(&A, NULL), FAILURE);

  s21_matrix C;
  C.matrix = NULL;
  ck_assert_int_eq(s21_eq_matrix(&A, &C), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_nan_fail) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = NAN;
  B.matrix[0][0] = 1.0;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// Тесты: Operations
START_TEST(sum_matrix_ok) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  B.matrix[0][0] = 3.0;
  B.matrix[0][1] = 4.0;

  int status = s21_sum_matrix(&A, &B, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 4.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][1], 6.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(sum_matrix_fail) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);

  ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), CALC_ERROR);
  ck_assert_int_eq(s21_sum_matrix(NULL, &B, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, NULL), INCORRECT_MATRIX);

  A.matrix = NULL;
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), INCORRECT_MATRIX);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_alloc_fail) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  test_create_matrix_fail_flag = 1;
  int status = s21_sum_matrix(&A, &B, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_ok) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 5.0;
  B.matrix[0][0] = 3.0;

  int status = s21_sub_matrix(&A, &B, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 2.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(sub_matrix_fail) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), CALC_ERROR);
  ck_assert_int_eq(s21_sub_matrix(NULL, &B, &res), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_alloc_fail) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  test_create_matrix_fail_flag = 1;
  int status = s21_sub_matrix(&A, &B, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mult_number_ok) {
  s21_matrix A, res;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 2.5;

  int status = s21_mult_number(&A, 2.0, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 5.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(mult_number_fail) {
  s21_matrix A, res;
  s21_create_matrix(2, 2, &A);

  ck_assert_int_eq(s21_mult_number(NULL, 2.0, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_mult_number(&A, 2.0, NULL), INCORRECT_MATRIX);

  A.matrix = NULL;
  ck_assert_int_eq(s21_mult_number(&A, 2.0, &res), INCORRECT_MATRIX);
}
END_TEST

START_TEST(mult_number_alloc_fail) {
  s21_matrix A, res;
  s21_create_matrix(2, 2, &A);
  test_create_matrix_fail_flag = 1;
  int status = s21_mult_number(&A, 2.0, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(mult_matrix_ok) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  B.matrix[0][0] = 4;
  B.matrix[1][0] = 5;
  B.matrix[2][0] = 6;

  int status = s21_mult_matrix(&A, &B, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 32.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(mult_matrix_fail) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 2, &B);

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), CALC_ERROR);
  ck_assert_int_eq(s21_mult_matrix(NULL, &B, &res), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mult_matrix_alloc_fail) {
  s21_matrix A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  test_create_matrix_fail_flag = 1;
  int status = s21_mult_matrix(&A, &B, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// Тесты: Transpose
START_TEST(transpose_ok) {
  s21_matrix A, res;
  s21_create_matrix(2, 3, &A);
  A.matrix[0][1] = 5.5;

  int status = s21_transpose(&A, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_int_eq(res.rows, 3);
  ck_assert_int_eq(res.columns, 2);
  ck_assert_double_eq_tol(res.matrix[1][0], 5.5, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(transpose_fail) {
  s21_matrix A, res;
  s21_create_matrix(2, 2, &A);

  ck_assert_int_eq(s21_transpose(NULL, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_transpose(&A, NULL), INCORRECT_MATRIX);

  A.matrix = NULL;
  ck_assert_int_eq(s21_transpose(&A, &res), INCORRECT_MATRIX);
}
END_TEST

START_TEST(transpose_alloc_fail) {
  s21_matrix A, res;
  s21_create_matrix(2, 2, &A);
  test_create_matrix_fail_flag = 1;
  int status = s21_transpose(&A, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

// Тесты: Determinant
START_TEST(determinant_ok) {
  s21_matrix A;
  double res = 0;

  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.5;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, 5.5, 1e-7);
  s21_remove_matrix(&A);

  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 4;
  A.matrix[0][1] = 3;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, 5.0, 1e-7);
  s21_remove_matrix(&A);

  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, -40.0, 1e-7);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_fail) {
  s21_matrix A;
  double res = 0;
  s21_create_matrix(2, 3, &A);

  ck_assert_int_eq(s21_determinant(&A, &res), CALC_ERROR);
  ck_assert_int_eq(s21_determinant(NULL, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_determinant(&A, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_internal_fail) {
  s21_matrix A;
  double result = 0;
  s21_create_matrix(3, 3, &A);

  A.columns = -1;
  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  A.columns = 3;
  s21_remove_matrix(&A);
}
END_TEST

// Тесты: Calc_Complements
START_TEST(calc_complements_ok) {
  s21_matrix A, res;

  s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(s21_calc_complements(&A, &res), OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 1.0, 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);

  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;

  ck_assert_int_eq(s21_calc_complements(&A, &res), OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 0.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][1], 10.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][2], -20.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(calc_complements_fail) {
  s21_matrix A, res;
  s21_create_matrix(2, 3, &A);

  ck_assert_int_eq(s21_calc_complements(&A, &res), CALC_ERROR);
  ck_assert_int_eq(s21_calc_complements(NULL, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_calc_complements(&A, NULL), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_minor_fail) {
  s21_matrix A, res;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;

  test_memory_fail_flag = 1;
  int status = s21_calc_complements(&A, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_det_fail) {
  s21_matrix A, res;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;

  test_det_fail_flag = 1;
  int status = s21_calc_complements(&A, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_result_create_fail) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);

  test_create_matrix_fail_flag = 1;
  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_internal_fail_dim) {
  s21_matrix A, res;
  s21_create_matrix(3, 3, &A);

  A.columns = -1;
  int status = s21_calc_complements(&A, &res);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  A.columns = 3;
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_internal_fail_rows) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);

  A.rows = -1;
  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  A.rows = 3;
  s21_remove_matrix(&A);
}
END_TEST

// Тесты: Inverse matrix
START_TEST(inverse_matrix_ok) {
  s21_matrix A, res;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  int status = s21_inverse_matrix(&A, &res);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][1], -1.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][2], 1.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(inverse_matrix_fail) {
  s21_matrix A, res;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  ck_assert_int_eq(s21_inverse_matrix(&A, &res), CALC_ERROR);
  ck_assert_int_eq(s21_inverse_matrix(NULL, &res), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_transpose_fail) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  test_transpose_fail_flag = 1;
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_det_zero) {
  s21_matrix A;
  s21_matrix result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_calc_complements_error) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  test_memory_fail_flag = 1;
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_det_error) {
  s21_matrix A, result;
  s21_create_matrix(3, 3, &A);

  test_det_fail_flag = 1;
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_not_square) {
  s21_matrix A, result;
  s21_create_matrix(2, 3, &A);
  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, CALC_ERROR);
  s21_remove_matrix(&A);
}
END_TEST

// Тесты: Helpers
START_TEST(helpers_check) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  ck_assert_int_eq(s21_is_correct_matrix(&A), 1);
  ck_assert_int_eq(s21_same_size(&A, &B), 1);
  ck_assert_int_eq(s21_is_square(&A), 1);

  A.matrix[0][0] = 123.45;
  s21_copy_matrix(&A, &B);
  ck_assert_double_eq_tol(B.matrix[0][0], 123.45, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(matrix_pointer_null_fail) {
  s21_matrix A;
  A.matrix = NULL;
  A.rows = 3;
  A.columns = 3;
  s21_matrix res;
  double det_res = 0;

  ck_assert_int_eq(s21_calc_complements(&A, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_determinant(&A, &det_res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_inverse_matrix(&A, &res), INCORRECT_MATRIX);
  ck_assert_int_eq(s21_transpose(&A, &res), INCORRECT_MATRIX);
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s = suite_create("s21_matrix_tests");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, create_matrix_ok);
  tcase_add_test(tc_core, create_matrix_fail);
  tcase_add_test(tc_core, create_matrix_row_alloc_fail);

  tcase_add_test(tc_core, eq_matrix_true);
  tcase_add_test(tc_core, eq_matrix_false);
  tcase_add_test(tc_core, eq_matrix_nan_fail);

  tcase_add_test(tc_core, sum_matrix_ok);
  tcase_add_test(tc_core, sum_matrix_fail);
  tcase_add_test(tc_core, sum_matrix_alloc_fail);

  tcase_add_test(tc_core, sub_matrix_ok);
  tcase_add_test(tc_core, sub_matrix_fail);
  tcase_add_test(tc_core, sub_matrix_alloc_fail);

  tcase_add_test(tc_core, mult_number_ok);
  tcase_add_test(tc_core, mult_number_fail);
  tcase_add_test(tc_core, mult_number_alloc_fail);

  tcase_add_test(tc_core, mult_matrix_ok);
  tcase_add_test(tc_core, mult_matrix_fail);
  tcase_add_test(tc_core, mult_matrix_alloc_fail);

  tcase_add_test(tc_core, transpose_ok);
  tcase_add_test(tc_core, transpose_fail);
  tcase_add_test(tc_core, transpose_alloc_fail);

  tcase_add_test(tc_core, determinant_ok);
  tcase_add_test(tc_core, determinant_fail);
  tcase_add_test(tc_core, determinant_internal_fail);

  tcase_add_test(tc_core, calc_complements_ok);
  tcase_add_test(tc_core, calc_complements_fail);
  tcase_add_test(tc_core, calc_complements_minor_fail);
  tcase_add_test(tc_core, calc_complements_det_fail);
  tcase_add_test(tc_core, calc_complements_result_create_fail);
  tcase_add_test(tc_core, calc_complements_internal_fail_dim);
  tcase_add_test(tc_core, calc_complements_internal_fail_rows);

  tcase_add_test(tc_core, inverse_matrix_ok);
  tcase_add_test(tc_core, inverse_matrix_fail);
  tcase_add_test(tc_core, inverse_matrix_transpose_fail);
  tcase_add_test(tc_core, inverse_matrix_det_zero);
  tcase_add_test(tc_core, inverse_matrix_calc_complements_error);
  tcase_add_test(tc_core, inverse_matrix_det_error);
  tcase_add_test(tc_core, inverse_matrix_not_square);

  tcase_add_test(tc_core, helpers_check);
  tcase_add_test(tc_core, matrix_pointer_null_fail);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s = matrix_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
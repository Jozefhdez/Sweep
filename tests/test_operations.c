#include "../include/sw_obj.h"
#include "../include/sw_ops.h"
#include <criterion/criterion.h>

Test(operations, int_addition) {
    sw_obj_t *a = sw_int(5);
    sw_obj_t *b = sw_int(3);
    sw_obj_t *result = sw_add(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_INT, "Result should be integer");
    cr_assert_eq(result->data.v_int, 8, "5 + 3 should equal 8");
}

Test(operations, float_addition) {
    sw_obj_t *a = sw_float(3.5f);
    sw_obj_t *b = sw_float(2.5f);
    sw_obj_t *result = sw_add(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_FLOAT, "Result should be float");
    cr_assert_float_eq(result->data.v_float, 6.0f, 0.01f,
                       "3.5 + 2.5 should equal 6.0");
}

Test(operations, type_coercion) {
    sw_obj_t *a = sw_int(5);
    sw_obj_t *b = sw_float(2.5f);
    sw_obj_t *result = sw_add(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_FLOAT,
                 "Result should be float after coercion");
    cr_assert_float_eq(result->data.v_float, 7.5f, 0.01f,
                       "5 + 2.5 should equal 7.5");
}

Test(operations, string_concatenation) {
    sw_obj_t *a = sw_string("Hello ");
    sw_obj_t *b = sw_string("World");
    sw_obj_t *result = sw_add(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_STRING, "Result should be string");
    cr_assert_str_eq(result->data.v_string, "Hello World",
                     "Strings should concatenate");
}

Test(operations, vector_addition) {
    sw_obj_t *v1 = sw_vec3(sw_int(1), sw_int(2), sw_int(3));
    sw_obj_t *v2 = sw_vec3(sw_int(4), sw_int(5), sw_int(6));
    sw_obj_t *result = sw_add(v1, v2);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_VEC3, "Result should be vector");
    cr_assert_eq(result->data.v_vec3.x->data.v_int, 5, "X: 1+4=5");
    cr_assert_eq(result->data.v_vec3.y->data.v_int, 7, "Y: 2+5=7");
    cr_assert_eq(result->data.v_vec3.z->data.v_int, 9, "Z: 3+6=9");
}

Test(operations, scalar_vector_multiplication) {
    sw_obj_t *scalar = sw_int(2);
    sw_obj_t *vec = sw_vec3(sw_int(10), sw_int(20), sw_int(30));
    sw_obj_t *result = sw_mul(scalar, vec);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_VEC3, "Result should be vector");
    cr_assert_eq(result->data.v_vec3.x->data.v_int, 20, "X: 2*10=20");
    cr_assert_eq(result->data.v_vec3.y->data.v_int, 40, "Y: 2*20=40");
    cr_assert_eq(result->data.v_vec3.z->data.v_int, 60, "Z: 2*30=60");
}

Test(operations, subtraction) {
    sw_obj_t *a = sw_int(10);
    sw_obj_t *b = sw_int(3);
    sw_obj_t *result = sw_sub(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_INT, "Result should be integer");
    cr_assert_eq(result->data.v_int, 7, "10 - 3 should equal 7");
}

Test(operations, multiplication) {
    sw_obj_t *a = sw_int(5);
    sw_obj_t *b = sw_int(3);
    sw_obj_t *result = sw_mul(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_INT, "Result should be integer");
    cr_assert_eq(result->data.v_int, 15, "5 * 3 should equal 15");
}

Test(operations, division) {
    sw_obj_t *a = sw_int(15);
    sw_obj_t *b = sw_int(3);
    sw_obj_t *result = sw_div(a, b);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_assert_eq(result->kind, SW_FLOAT,
                 "Result should be float (division always returns float)");
    cr_assert_float_eq(result->data.v_float, 5.0f, 0.01f,
                       "15 / 3 should equal 5.0");
}

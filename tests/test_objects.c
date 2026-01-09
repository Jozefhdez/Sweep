#include "../include/sw_obj.h"
#include <criterion/criterion.h>

Test(objects, create_int) {
    sw_obj_t *obj = sw_int(42);

    cr_assert_not_null(obj, "Object should not be NULL");
    cr_assert_eq(obj->kind, SW_INT, "Object kind should be SW_INT");
    cr_assert_eq(obj->data.v_int, 42, "Integer value should be 42");

    free(obj);
}

Test(objects, create_float) {
    sw_obj_t *obj = sw_float(3.14f);

    cr_assert_not_null(obj, "Object should not be NULL");
    cr_assert_eq(obj->kind, SW_FLOAT, "Object kind should be SW_FLOAT");
    cr_assert_float_eq(obj->data.v_float, 3.14f, 0.01f,
                       "Float value should be 3.14");

    free(obj);
}

Test(objects, create_string) {
    sw_obj_t *obj = sw_string("Hello");

    cr_assert_not_null(obj, "Object should not be NULL");
    cr_assert_eq(obj->kind, SW_STRING, "Object kind should be SW_STRING");
    cr_assert_str_eq(obj->data.v_string, "Hello",
                     "String value should be 'Hello'");

    free(obj->data.v_string);
    free(obj);
}

Test(objects, create_vec3) {
    sw_obj_t *x = sw_int(1);
    sw_obj_t *y = sw_int(2);
    sw_obj_t *z = sw_int(3);
    sw_obj_t *vec = sw_vec3(x, y, z);

    cr_assert_not_null(vec, "Vector should not be NULL");
    cr_assert_eq(vec->kind, SW_VEC3, "Object kind should be SW_VEC3");
    cr_assert_eq(vec->data.v_vec3.x, x, "X component should match");
    cr_assert_eq(vec->data.v_vec3.y, y, "Y component should match");
    cr_assert_eq(vec->data.v_vec3.z, z, "Z component should match");

    free(x);
    free(y);
    free(z);
    free(vec);
}

Test(objects, create_array) {
    sw_obj_t *arr = sw_array(5);

    cr_assert_not_null(arr, "Array should not be NULL");
    cr_assert_eq(arr->kind, SW_ARRAY, "Object kind should be SW_ARRAY");
    cr_assert_eq(arr->data.v_array.size, 5, "Array size should be 5");
    cr_assert_not_null(arr->data.v_array.elements,
                       "Array elements should not be NULL");

    free(arr->data.v_array.elements);
    free(arr);
}

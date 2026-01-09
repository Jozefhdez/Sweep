#include "sw_array.h"
#include "sw_obj.h"
#include "sw_ops.h"
#include <stdio.h>

int main() {
    printf("Testing sw_add\n\n");

    sw_obj_t *int1 = sw_int(10);
    sw_obj_t *int2 = sw_int(20);
    sw_obj_t *int_sum = sw_add(int1, int2);
    printf("Integer: 10 + 20 = %d\n", int_sum->data.v_int);

    sw_obj_t *float1 = sw_float(3.5);
    sw_obj_t *float2 = sw_float(2.5);
    sw_obj_t *float_sum = sw_add(float1, float2);
    printf("Float: 3.5 + 2.5 = %f\n", float_sum->data.v_float);

    sw_obj_t *int_float_sum = sw_add(int1, float1);
    printf("Int + Float: 10 + 3.5 = %f\n", int_float_sum->data.v_float);

    sw_obj_t *str1 = sw_string("Hello ");
    sw_obj_t *str2 = sw_string("World!");
    sw_obj_t *str_sum = sw_add(str1, str2);
    printf("String: \"Hello \" + \"World!\" = \"%s\"\n",
           str_sum->data.v_string);

    sw_obj_t *v1 = sw_vec3(int1, int2, sw_int(30));
    sw_obj_t *v2 = sw_vec3(sw_int(1), sw_int(2), sw_int(3));
    sw_obj_t *vec_sum = sw_add(v1, v2);
    printf("Vector3: [10,20,30] + [1,2,3] = [%d,%d,%d]\n",
           vec_sum->data.v_vec3.x->data.v_int,
           vec_sum->data.v_vec3.y->data.v_int,
           vec_sum->data.v_vec3.z->data.v_int);

    sw_obj_t *arr1 = sw_array(2);
    sw_array_set(arr1, 0, sw_int(1));
    sw_array_set(arr1, 1, sw_int(2));

    sw_obj_t *arr2 = sw_array(3);
    sw_array_set(arr2, 0, sw_int(3));
    sw_array_set(arr2, 1, sw_int(4));
    sw_array_set(arr2, 2, sw_int(5));

    sw_obj_t *arr_sum = sw_add(arr1, arr2);
    printf("Array: [1,2] + [3,4,5] = [");
    for (size_t i = 0; i < arr_sum->data.v_array.size; i++) {
        printf("%d", sw_array_get(arr_sum, i)->data.v_int);
        if (i < arr_sum->data.v_array.size - 1)
            printf(",");
    }
    printf("]\n");

    printf("\nTesting sw_sub\n\n");

    sw_obj_t *int_sub = sw_sub(int1, int2);
    printf("Integer: 10 - 20 = %d\n", int_sub->data.v_int);

    sw_obj_t *float_sub = sw_sub(float1, float2);
    printf("Float: 3.5 - 2.5 = %.1f\n", float_sub->data.v_float);

    sw_obj_t *int_float_sub = sw_sub(int1, float1);
    printf("Int - Float: 10 - 3.5 = %.1f\n", int_float_sub->data.v_float);

    sw_obj_t *vec_sub = sw_sub(v1, v2);
    printf("Vector3: [10,20,30] - [1,2,3] = [%d,%d,%d]\n",
           vec_sub->data.v_vec3.x->data.v_int,
           vec_sub->data.v_vec3.y->data.v_int,
           vec_sub->data.v_vec3.z->data.v_int);

    printf("\nTesting sw_mul\n\n");

    sw_obj_t *int_mul = sw_mul(int1, int2);
    printf("Integer: 10 * 20 = %d\n", int_mul->data.v_int);

    sw_obj_t *float_mul = sw_mul(float1, float2);
    printf("Float: 3.5 * 2.5 = %f\n", float_mul->data.v_float);

    sw_obj_t *int_float_mul = sw_mul(int1, float1);
    printf("Int * Float: 10 * 3.5 = %f\n", int_float_mul->data.v_float);

    sw_obj_t *vec_mul = sw_mul(v1, v2);
    printf("Vector3: [10,20,30] * [1,2,3] = [%d,%d,%d]\n",
           vec_mul->data.v_vec3.x->data.v_int,
           vec_mul->data.v_vec3.y->data.v_int,
           vec_mul->data.v_vec3.z->data.v_int);

    printf("\nTesting sw_div\n\n");

    sw_obj_t *int_div = sw_div(int1, int2);
    if (int_div) {
        printf("Integer: 10 / 20 = %f\n", int_div->data.v_float);
    } else {
        printf("Integer: 10 / 20 = Division by zero or error\n");
    }

    sw_obj_t *float_div = sw_div(float1, float2);
    if (float_div) {
        printf("Float: 3.5 / 2.5 = %f\n", float_div->data.v_float);
    } else {
        printf("Float: 3.5 / 2.5 = Error\n");
    }

    sw_obj_t *int_float_div = sw_div(int1, float1);
    if (int_float_div) {
        printf("Int / Float: 10 / 3.5 = %f\n", int_float_div->data.v_float);
    } else {
        printf("Int / Float: 10 / 3.5 = Error\n");
    }

    sw_obj_t *vec_div = sw_div(v1, v2);
    if (vec_div) {
        printf("Vector3: [10,20,30] / [1,2,3] = [%f,%f,%f]\n",
               vec_div->data.v_vec3.x->data.v_float,
               vec_div->data.v_vec3.y->data.v_float,
               vec_div->data.v_vec3.z->data.v_float);
    } else {
        printf("Vector3: Division error (possibly by zero)\n");
    }

    return 0;
}

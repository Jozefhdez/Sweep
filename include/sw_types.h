#ifndef SW_TYPES_H
#define SW_TYPES_H

#include <stdbool.h>
#include <stddef.h>

typedef enum { SW_INT, SW_FLOAT, SW_STRING, SW_VEC3, SW_ARRAY } sw_kind_t;

typedef struct sw_obj sw_obj_t;

typedef struct sw_vec3 {
    sw_obj_t *x;
    sw_obj_t *y;
    sw_obj_t *z;
} sw_vec3_t;

typedef struct sw_array {
    size_t size;
    sw_obj_t **elements;
} sw_array_t;

typedef union {
    int v_int;
    float v_float;
    char *v_string;
    sw_vec3_t v_vec3;
    sw_array_t v_array;
} sw_data_t;

struct sw_obj {
    sw_kind_t kind;
    sw_data_t data;
    int refcount;
};

#endif

#include "sw_ops.h"
#include "sw_array.h"
#include "sw_obj.h"
#include <stdlib.h>
#include <string.h>

int sw_len(sw_obj_t *obj) {
    if (obj == NULL) {
        return -1;
    }

    switch (obj->kind) {
    case SW_INT:
        return 1;
    case SW_FLOAT:
        return 1;
    case SW_STRING:
        return strlen(obj->data.v_string);
    case SW_VEC3:
        return 3;
    case SW_ARRAY:
        return obj->data.v_array.size;
    default:
        return -1;
    }
}

sw_obj_t *sw_add(sw_obj_t *a, sw_obj_t *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    switch (a->kind) {
    case SW_INT:
        if (b->kind == SW_INT) {
            return sw_int(a->data.v_int + b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            return sw_float(a->data.v_int + b->data.v_float);
        } else if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_add(a, b->data.v_vec3.x);
            sw_obj_t *y = sw_add(a, b->data.v_vec3.y);
            sw_obj_t *z = sw_add(a, b->data.v_vec3.z);
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    case SW_FLOAT:
        if (b->kind == SW_INT) {
            return sw_float(a->data.v_float + b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            return sw_float(a->data.v_float + b->data.v_float);
        } else {
            return NULL;
        }
        break;
    case SW_STRING:
        if (b->kind != SW_STRING) {
            return NULL;
        } else {
            size_t length =
                strlen(a->data.v_string) + strlen(b->data.v_string) + 1;

            char *temp_string = (char *)calloc(length, sizeof(char));
            strcat(temp_string, a->data.v_string);
            strcat(temp_string, b->data.v_string);

            sw_obj_t *new_string = sw_string(temp_string);
            free(temp_string);
            return new_string;
        }
        break;
    case SW_VEC3:
        if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_add(a->data.v_vec3.x, b->data.v_vec3.x);
            sw_obj_t *y = sw_add(a->data.v_vec3.y, b->data.v_vec3.y);
            sw_obj_t *z = sw_add(a->data.v_vec3.z, b->data.v_vec3.z);
            return sw_vec3(x, y, z);
        } else if (b->kind == SW_INT) {
            sw_obj_t *x = sw_add(a->data.v_vec3.x, b);
            sw_obj_t *y = sw_add(a->data.v_vec3.y, b);
            sw_obj_t *z = sw_add(a->data.v_vec3.z, b);
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    case SW_ARRAY:
        if (b->kind != SW_ARRAY) {
            return NULL;
        } else {
            size_t size_a = a->data.v_array.size;
            size_t size_b = b->data.v_array.size;
            sw_obj_t *arr = sw_array(size_a + size_b);

            for (size_t i = 0; i < size_a; i++) {
                sw_array_set(arr, i, sw_array_get(a, i));
            }

            for (size_t i = 0; i < size_b; i++) {
                sw_array_set(arr, size_a + i, sw_array_get(b, i));
            }

            return arr;
        }
        break;
    default:
        return NULL;
        break;
    }
}

sw_obj_t *sw_sub(sw_obj_t *a, sw_obj_t *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    switch (a->kind) {
    case SW_INT:
        if (b->kind == SW_INT) {
            return sw_int(a->data.v_int - b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            return sw_float(a->data.v_int - b->data.v_float);
        } else if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_sub(a, b->data.v_vec3.x);
            sw_obj_t *y = sw_sub(a, b->data.v_vec3.y);
            sw_obj_t *z = sw_sub(a, b->data.v_vec3.z);
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    case SW_FLOAT:
        if (b->kind == SW_INT) {
            return sw_float(a->data.v_float - b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            return sw_float(a->data.v_float - b->data.v_float);
        } else {
            return NULL;
        }
        break;
    case SW_STRING:
        return NULL;
        break;
    case SW_VEC3:
        if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_sub(a->data.v_vec3.x, b->data.v_vec3.x);
            sw_obj_t *y = sw_sub(a->data.v_vec3.y, b->data.v_vec3.y);
            sw_obj_t *z = sw_sub(a->data.v_vec3.z, b->data.v_vec3.z);
            return sw_vec3(x, y, z);
        } else if (b->kind == SW_INT) {
            sw_obj_t *x = sw_sub(a->data.v_vec3.x, b);
            sw_obj_t *y = sw_sub(a->data.v_vec3.y, b);
            sw_obj_t *z = sw_sub(a->data.v_vec3.z, b);
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    case SW_ARRAY:
        return NULL;
        break;
    default:
        return NULL;
        break;
    }
}

sw_obj_t *sw_mul(sw_obj_t *a, sw_obj_t *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    switch (a->kind) {
    case SW_INT:
        if (b->kind == SW_INT) {
            return sw_int(a->data.v_int * b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            return sw_float(a->data.v_int * b->data.v_float);
        } else if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_mul(a, b->data.v_vec3.x);
            sw_obj_t *y = sw_mul(a, b->data.v_vec3.y);
            sw_obj_t *z = sw_mul(a, b->data.v_vec3.z);
            return sw_vec3(x, y, z);
        } else if (b->kind == SW_ARRAY) {
            sw_obj_t *result = sw_array(b->data.v_array.size);
            for (size_t i = 0; i < b->data.v_array.size; i++) {
                sw_obj_t *elem = sw_array_get(b, i);
                sw_obj_t *multiplied = sw_mul(a, elem);
                sw_array_set(result, i, multiplied);
            }
            return result;
        } else {
            return NULL;
        }
        break;
    case SW_FLOAT:
        if (b->kind == SW_INT) {
            return sw_int(a->data.v_float * b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            return sw_float(a->data.v_float * b->data.v_float);
        } else if (b->kind == SW_ARRAY) {
            sw_obj_t *result = sw_array(b->data.v_array.size);
            for (size_t i = 0; i < b->data.v_array.size; i++) {
                sw_obj_t *elem = sw_array_get(b, i);
                sw_obj_t *multiplied = sw_mul(a, elem);
                sw_array_set(result, i, multiplied);
            }
            return result;
        } else {
            return NULL;
        }
        break;
    case SW_ARRAY:
        if (b->kind == SW_INT || b->kind == SW_FLOAT) {
            sw_obj_t *result = sw_array(a->data.v_array.size);
            for (size_t i = 0; i < a->data.v_array.size; i++) {
                sw_obj_t *elem = sw_array_get(a, i);
                sw_obj_t *multiplied = sw_mul(elem, b);
                sw_array_set(result, i, multiplied);
            }
            return result;
        } else {
            return NULL;
        }
        break;
    case SW_VEC3:
        if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_mul(a->data.v_vec3.x, b->data.v_vec3.x);
            sw_obj_t *y = sw_mul(a->data.v_vec3.y, b->data.v_vec3.y);
            sw_obj_t *z = sw_mul(a->data.v_vec3.z, b->data.v_vec3.z);
            return sw_vec3(x, y, z);
        } else if (b->kind == SW_INT) {
            sw_obj_t *x = sw_mul(a->data.v_vec3.x, b);
            sw_obj_t *y = sw_mul(a->data.v_vec3.y, b);
            sw_obj_t *z = sw_mul(a->data.v_vec3.z, b);
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    default:
        return NULL;
        break;
    }
}

sw_obj_t *sw_div(sw_obj_t *a, sw_obj_t *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    switch (a->kind) {
    case SW_INT:
        if (b->kind == SW_INT) {
            if (b->data.v_int == 0)
                return NULL;
            return sw_float((float)a->data.v_int / (float)b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            if (b->data.v_float == 0.0f)
                return NULL;
            return sw_float((float)a->data.v_int / b->data.v_float);
        } else if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_div(a, b->data.v_vec3.x);
            sw_obj_t *y = sw_div(a, b->data.v_vec3.y);
            sw_obj_t *z = sw_div(a, b->data.v_vec3.z);
            if (x == NULL || y == NULL || z == NULL)
                return NULL;
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    case SW_FLOAT:
        if (b->kind == SW_INT) {
            if (b->data.v_int == 0)
                return NULL;
            return sw_float(a->data.v_float / (float)b->data.v_int);
        } else if (b->kind == SW_FLOAT) {
            if (b->data.v_float == 0.0f)
                return NULL;
            return sw_float(a->data.v_float / b->data.v_float);
        } else {
            return NULL;
        }
        break;
    case SW_ARRAY:
        return NULL;
        break;
    case SW_VEC3:
        if (b->kind == SW_VEC3) {
            sw_obj_t *x = sw_div(a->data.v_vec3.x, b->data.v_vec3.x);
            sw_obj_t *y = sw_div(a->data.v_vec3.y, b->data.v_vec3.y);
            sw_obj_t *z = sw_div(a->data.v_vec3.z, b->data.v_vec3.z);
            if (x == NULL || y == NULL || z == NULL)
                return NULL;
            return sw_vec3(x, y, z);
        } else if (b->kind == SW_INT) {
            sw_obj_t *x = sw_div(a->data.v_vec3.x, b);
            sw_obj_t *y = sw_div(a->data.v_vec3.y, b);
            sw_obj_t *z = sw_div(a->data.v_vec3.z, b);
            if (x == NULL || y == NULL || z == NULL)
                return NULL;
            return sw_vec3(x, y, z);
        } else {
            return NULL;
        }
        break;
    default:
        return NULL;
        break;
    }
}

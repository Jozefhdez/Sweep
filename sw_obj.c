#include "sw_obj.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sw_obj_t *_new_sw_object(vm_t *vm) {
    sw_obj_t *sw_obj = calloc(1, sizeof(sw_obj_t));

    if (sw_obj == NULL) {
        return NULL;
    }

    vm_track_object(vm, sw_obj);
    sw_obj->marked = false;

    return sw_obj;
}

void sw_object_free(sw_obj_t *obj) {
    if (obj == NULL) {
        return;
    }

    switch (obj->kind) {
    case SW_INT:
        free(obj);
        break;
    case SW_FLOAT:
        free(obj);
        break;
    case SW_STRING:
        free(obj->data.v_string);
        free(obj);
        break;
    case SW_VEC3:
        free(obj);
        break;
    case SW_ARRAY:
        free(obj->data.v_array.elements);
        free(obj);
        break;
    case SW_FUNCTION:
        // free params
        for (int i = 0; i < obj->data.v_function.param_count; i++) {
            free(obj->data.v_function.params[i]);
        }
        free(obj->data.v_function.params);
        // body is AST, not freed here
        free(obj);
        break;
    }
}

sw_obj_t *sw_int(int value) {
    sw_obj_t *obj = (sw_obj_t *)malloc(sizeof(sw_obj_t));

    if (obj == NULL) {
        return NULL;
    }
    obj->kind = SW_INT;
    obj->data.v_int = value;

    return obj;
}

sw_obj_t *sw_float(float value) {
    sw_obj_t *obj = (sw_obj_t *)malloc(sizeof(sw_obj_t));

    if (obj == NULL) {
        return NULL;
    }
    obj->kind = SW_FLOAT;
    obj->data.v_float = value;

    return obj;
}

sw_obj_t *sw_string(char *value) {
    sw_obj_t *obj = (sw_obj_t *)malloc(sizeof(sw_obj_t));

    if (obj == NULL) {
        return NULL;
    }

    size_t length = strlen(value) + 1;
    obj->data.v_string = (char *)malloc(length);

    if (obj->data.v_string == NULL) {
        free(obj);
        return NULL;
    }

    obj->kind = SW_STRING;
    strcpy(obj->data.v_string, value);

    return obj;
}

sw_obj_t *sw_vec3(sw_obj_t *x, sw_obj_t *y, sw_obj_t *z) {
    if (x == NULL || y == NULL || z == NULL) {
        return NULL;
    }

    sw_obj_t *obj = (sw_obj_t *)malloc(sizeof(sw_obj_t));

    if (obj == NULL) {
        return NULL;
    }

    obj->kind = SW_VEC3;
    sw_vec3_t vec = (sw_vec3_t){.x = x, .y = y, .z = z};
    obj->data.v_vec3 = vec;

    return obj;
}

sw_obj_t *sw_array(size_t size) {
    sw_obj_t *obj = malloc(sizeof(sw_obj_t));

    if (obj == NULL) {
        return NULL;
    }

    sw_obj_t **elements = calloc(size, sizeof(sw_obj_t *));
    if (elements == NULL) {
        free(obj);
        return NULL;
    }
    obj->kind = SW_ARRAY;
    sw_array_t arr = (sw_array_t){.size = size, .elements = elements};
    obj->data.v_array = arr;

    return obj;
}

sw_obj_t *sw_function(void *body, char **params, int param_count) {
    sw_obj_t *obj = malloc(sizeof(sw_obj_t));

    if (obj == NULL) {
        return NULL;
    }

    obj->kind = SW_FUNCTION;
    obj->data.v_function.body = body;
    obj->data.v_function.params = params;
    obj->data.v_function.param_count = param_count;

    return obj;
}

void sw_print(sw_obj_t *obj) {
    if (!obj) {
        printf("NULL");
        return;
    }
    switch (obj->kind) {
    case SW_INT:
        printf("%d", obj->data.v_int);
        break;
    case SW_FLOAT:
        printf("%f", obj->data.v_float);
        break;
    case SW_STRING:
        printf("\"%s\"", obj->data.v_string);
        break;
    case SW_VEC3:
        printf("(");
        sw_print(obj->data.v_vec3.x);
        printf(", ");
        sw_print(obj->data.v_vec3.y);
        printf(", ");
        sw_print(obj->data.v_vec3.z);
        printf(")");
        break;
    case SW_ARRAY:
        printf("[");
        for (size_t i = 0; i < obj->data.v_array.size; i++) {
            sw_print(obj->data.v_array.elements[i]);
            if (i < obj->data.v_array.size - 1)
                printf(", ");
        }
        printf("]");
        break;
    case SW_FUNCTION:
        printf("<function>");
        break;
    default:
        printf("UNKNOWN");
    }
}

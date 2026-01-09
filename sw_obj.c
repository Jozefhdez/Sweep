#include "sw_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    default:
        printf("UNKNOWN");
    }
}

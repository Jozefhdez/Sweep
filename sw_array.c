#include "sw_array.h"

bool sw_array_set(sw_obj_t *obj, size_t index, sw_obj_t *value) {
    if (obj == NULL || value == NULL || obj->kind != SW_ARRAY ||
        index >= obj->data.v_array.size) {
        return false;
    }

    obj->data.v_array.elements[index] = value;
    return true;
}

sw_obj_t *sw_array_get(sw_obj_t *obj, size_t index) {
    if (obj == NULL || obj->kind != SW_ARRAY ||
        index >= obj->data.v_array.size) {
        return NULL;
    }

    return obj->data.v_array.elements[index];
}

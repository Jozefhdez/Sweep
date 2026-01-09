#include "sw_array.h"
#include "sw_obj.h"

bool sw_array_set(sw_obj_t *obj, size_t index, sw_obj_t *value) {
    if (obj == NULL || value == NULL || obj->kind != SW_ARRAY ||
        index >= obj->data.v_array.size) {
        return false;
    }

    if (obj->data.v_array.elements[index]) {
        refcount_dec(obj->data.v_array.elements[index]);
    }

    obj->data.v_array.elements[index] = value;
    refcount_inc(obj->data.v_array.elements[index]);
    return true;
}

sw_obj_t *sw_array_get(sw_obj_t *obj, size_t index) {
    if (obj == NULL || obj->kind != SW_ARRAY ||
        index >= obj->data.v_array.size) {
        return NULL;
    }

    return obj->data.v_array.elements[index];
}

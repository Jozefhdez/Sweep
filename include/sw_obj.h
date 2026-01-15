#ifndef SW_OBJ_H
#define SW_OBJ_H

#include "sw_gc.h"
#include "sw_types.h"

sw_obj_t *_new_sw_object(vm_t *vm);
void sw_object_free(sw_obj_t *obj);
sw_obj_t *sw_int(int value);
sw_obj_t *sw_float(float value);
sw_obj_t *sw_string(char *value);
sw_obj_t *sw_vec3(sw_obj_t *x, sw_obj_t *y, sw_obj_t *z);
sw_obj_t *sw_array(size_t size);
void sw_print(sw_obj_t *obj);
sw_obj_t *sw_function(void *body, char **params, int param_count);

#endif

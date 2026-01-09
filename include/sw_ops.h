#ifndef SW_OPS_H
#define SW_OPS_H

#include "sw_types.h"

int sw_len(sw_obj_t *obj);
sw_obj_t *sw_add(sw_obj_t *a, sw_obj_t *b);
sw_obj_t *sw_sub(sw_obj_t *a, sw_obj_t *b);
sw_obj_t *sw_mul(sw_obj_t *a, sw_obj_t *b);
sw_obj_t *sw_div(sw_obj_t *a, sw_obj_t *b);

#endif

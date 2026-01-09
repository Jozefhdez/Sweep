#ifndef SW_ARRAY_H
#define SW_ARRAY_H

#include "sw_types.h"

bool sw_array_set(sw_obj_t *obj, size_t index, sw_obj_t *value);
sw_obj_t *sw_array_get(sw_obj_t *obj, size_t index);

#endif

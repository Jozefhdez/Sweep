#ifndef SW_GC_H
#define SW_GC_H

#include "sw_stack.h"
#include "sw_types.h"

typedef struct VirtualMachine {
    obj_stack_t *objects;
} vm_t;

vm_t *vm_new(void);
void vm_free(vm_t *vm);
void vm_track_object(vm_t *vm, sw_obj_t *obj);
void vm_set_current(vm_t *vm);
vm_t *vm_get_current(void);
void mark(vm_t *vm);
void trace(vm_t *vm);
void trace_blacken_object(obj_stack_t *gray_objects, sw_obj_t *obj);
void trace_mark_object(obj_stack_t *gray_objects, sw_obj_t *obj);
void sweep(vm_t *vm);
void vm_collect_garbage(vm_t *vm);

#endif
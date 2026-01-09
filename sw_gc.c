#include "include/sw_gc.h"
#include "include/sw_interpreter.h"
#include "include/sw_obj.h"
#include <stdlib.h>

vm_t *vm_new(void) {
    vm_t *vm = malloc(sizeof(vm_t));
    vm->objects = stack_new(8);

    return vm;
}

void vm_free(vm_t *vm) {
    for (size_t i = 0; i < vm->objects->count; i++) {
        sw_object_free(vm->objects->data[i]);
    }

    stack_free(vm->objects);

    free(vm);
}

void vm_track_object(vm_t *vm, sw_obj_t *obj) {
    stack_push(vm->objects, obj);
}

void mark(vm_t *vm) {
    (void)vm; // for now
    // get the symbol table
    sw_obj_t **root_objects;
    int root_count;
    get_symbol_table(&root_objects, &root_count);

    for (int i = 0; i < root_count; i++) {
        if (root_objects[i] != NULL) {
            // mark the object
            root_objects[i]->marked = true;
        }
    }
}

void trace(vm_t *vm) {
    obj_stack_t *gray_objects = stack_new(8);
    if (gray_objects == NULL) {
        return;
    }

    for (size_t i = 0; i < vm->objects->count; i++) {
        sw_obj_t *obj = (sw_obj_t *)vm->objects->data[i];
        if (obj != NULL && obj->marked) {
            stack_push(gray_objects, obj);
        }
    }

    while (gray_objects->count > 0) {
        sw_obj_t *obj = (sw_obj_t *)stack_pop(gray_objects);

        trace_blacken_object(gray_objects, obj);
    }

    stack_free(gray_objects);
}

void trace_blacken_object(obj_stack_t *gray_objects, sw_obj_t *obj) {
    switch (obj->kind) {
    case SW_VEC3:
        trace_mark_object(gray_objects, obj->data.v_vec3.x);
        trace_mark_object(gray_objects, obj->data.v_vec3.y);
        trace_mark_object(gray_objects, obj->data.v_vec3.z);
        break;
    case SW_ARRAY:
        for (size_t i = 0; i < obj->data.v_array.size; i++) {
            trace_mark_object(gray_objects, obj->data.v_array.elements[i]);
        }
        break;
    default:
        break;
    }
}

void trace_mark_object(obj_stack_t *gray_objects, sw_obj_t *obj) {
    if (obj == NULL || obj->marked) {
        return;
    }

    obj->marked = true;
    stack_push(gray_objects, obj);
}

void sweep(vm_t *vm) {
    for (size_t i = 0; i < vm->objects->count; i++) {
        sw_obj_t *obj = vm->objects->data[i];
        if (obj->marked) {
            obj->marked = false;
        } else {
            sw_object_free(obj);
            vm->objects->data[i] = NULL;
        }
    }
    stack_remove_nulls(vm->objects);
}

void vm_collect_garbage(vm_t *vm) {
    mark(vm);
    trace(vm);
    sweep(vm);
}
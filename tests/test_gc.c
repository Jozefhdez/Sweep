#include "../include/sw_array.h"
#include "../include/sw_gc.h"
#include "../include/sw_interpreter.h"
#include "../include/sw_obj.h"
#include <criterion/criterion.h>
#include <criterion/logging.h>

Test(gc, vm_creation) {
    vm_t *vm = vm_new();

    cr_assert_not_null(vm, "VM should be created");
    cr_assert_not_null(vm->objects, "VM objects stack should be initialized");
    cr_assert_eq(vm->objects->count, 0, "VM should start with 0 objects");

    vm_free(vm);
}

Test(gc, track_objects) {
    vm_t *vm = vm_new();

    sw_obj_t *obj1 = sw_int(42);
    sw_obj_t *obj2 = sw_int(100);
    sw_obj_t *obj3 = sw_string("test");

    vm_track_object(vm, obj1);
    vm_track_object(vm, obj2);
    vm_track_object(vm, obj3);

    cr_assert_eq(vm->objects->count, 3, "VM should track 3 objects");

    vm_free(vm);
}

Test(gc, trace_references_array) {
    vm_t *vm = vm_new();

    sw_obj_t *elem1 = sw_int(1);
    sw_obj_t *elem2 = sw_int(2);
    sw_obj_t *elem3 = sw_int(3);

    sw_obj_t *arr = sw_array(3);
    sw_array_set(arr, 0, elem1);
    sw_array_set(arr, 1, elem2);
    sw_array_set(arr, 2, elem3);

    vm_track_object(vm, arr);
    vm_track_object(vm, elem1);
    vm_track_object(vm, elem2);
    vm_track_object(vm, elem3);

    arr->marked = true;
    trace(vm);

    cr_assert_eq(elem1->marked, true,
                 "Array element should be marked by trace");
    cr_assert_eq(elem2->marked, true,
                 "Array element should be marked by trace");
    cr_assert_eq(elem3->marked, true,
                 "Array element should be marked by trace");

    vm_free(vm);
}

Test(gc, trace_vec3_references) {
    vm_t *vm = vm_new();

    sw_obj_t *x = sw_float(1.0);
    sw_obj_t *y = sw_float(2.0);
    sw_obj_t *z = sw_float(3.0);

    sw_obj_t *vec = sw_vec3(x, y, z);

    vm_track_object(vm, vec);
    vm_track_object(vm, x);
    vm_track_object(vm, y);
    vm_track_object(vm, z);

    vec->marked = true;
    trace(vm);

    cr_assert_eq(x->marked, true, "Vec3 x component should be marked");
    cr_assert_eq(y->marked, true, "Vec3 y component should be marked");
    cr_assert_eq(z->marked, true, "Vec3 z component should be marked");

    vm_free(vm);
}

Test(gc, sweep_unmarked_objects) {
    vm_t *vm = vm_new();

    sw_obj_t *obj1 = sw_int(42);
    sw_obj_t *obj2 = sw_int(100);
    sw_obj_t *obj3 = sw_int(200);

    vm_track_object(vm, obj1);
    vm_track_object(vm, obj2);
    vm_track_object(vm, obj3);

    cr_assert_eq(vm->objects->count, 3, "Should have 3 objects before sweep");

    obj1->marked = true;
    obj3->marked = true;

    sweep(vm);

    cr_assert_eq(vm->objects->count, 2, "Should have 2 objects after sweep");
    cr_assert_eq(obj1->marked, false, "Marks should be reset after sweep");
    cr_assert_eq(obj3->marked, false, "Marks should be reset after sweep");

    vm_free(vm);
}

Test(gc, gc_with_nested_structures) {
    vm_t *vm = vm_new();

    sw_obj_t *x = sw_float(1.0);
    sw_obj_t *y = sw_float(2.0);
    sw_obj_t *z = sw_float(3.0);
    sw_obj_t *vec = sw_vec3(x, y, z);

    sw_obj_t *arr = sw_array(1);
    sw_array_set(arr, 0, vec);

    sw_obj_t *garbage = sw_int(999);

    vm_track_object(vm, arr);
    vm_track_object(vm, vec);
    vm_track_object(vm, x);
    vm_track_object(vm, y);
    vm_track_object(vm, z);
    vm_track_object(vm, garbage);

    cr_assert_eq(vm->objects->count, 6, "Should have 6 objects before GC");

    arr->marked = true;

    trace(vm);
    sweep(vm);

    cr_assert_eq(vm->objects->count, 5,
                 "Should have 5 objects after GC (arr, vec, x, y, z)");

    vm_free(vm);
}

Test(gc, gc_all_garbage) {
    vm_t *vm = vm_new();

    sw_obj_t *obj1 = sw_int(1);
    sw_obj_t *obj2 = sw_int(2);
    sw_obj_t *obj3 = sw_int(3);

    vm_track_object(vm, obj1);
    vm_track_object(vm, obj2);
    vm_track_object(vm, obj3);

    cr_assert_eq(vm->objects->count, 3, "Should have 3 objects before GC");

    vm_collect_garbage(vm);

    cr_assert_eq(vm->objects->count, 0,
                 "Should have 0 objects after GC (all garbage)");

    vm_free(vm);
}

Test(gc, mark_empty_symbols) {
    vm_t *vm = vm_new();

    sw_obj_t *obj1 = sw_int(1);
    vm_track_object(vm, obj1);

    mark(vm);

    cr_assert_eq(obj1->marked, false,
                 "Object should not be marked without roots");

    vm_free(vm);
}

Test(gc, full_garbage_collection_cycle) {
    vm_t *vm = vm_new();

    sw_obj_t *obj1 = sw_int(10);
    sw_obj_t *obj2 = sw_int(20);
    sw_obj_t *obj3 = sw_int(30);
    sw_obj_t *obj4 = sw_int(40);

    sw_obj_t *arr = sw_array(2);
    sw_array_set(arr, 0, obj1);
    sw_array_set(arr, 1, obj2);

    vm_track_object(vm, obj1);
    vm_track_object(vm, obj2);
    vm_track_object(vm, obj3);
    vm_track_object(vm, obj4);
    vm_track_object(vm, arr);

    cr_assert_eq(vm->objects->count, 5, "Should have 5 objects");

    arr->marked = true;

    trace(vm);
    sweep(vm);

    cr_assert_eq(vm->objects->count, 3, "Should have 3 objects after GC");

    vm_free(vm);
}

Test(gc, trace_nested_arrays) {
    vm_t *vm = vm_new();

    sw_obj_t *inner1 = sw_int(1);
    sw_obj_t *inner2 = sw_int(2);
    sw_obj_t *inner_arr = sw_array(2);
    sw_array_set(inner_arr, 0, inner1);
    sw_array_set(inner_arr, 1, inner2);

    sw_obj_t *outer_arr = sw_array(1);
    sw_array_set(outer_arr, 0, inner_arr);

    vm_track_object(vm, outer_arr);
    vm_track_object(vm, inner_arr);
    vm_track_object(vm, inner1);
    vm_track_object(vm, inner2);

    outer_arr->marked = true;
    trace(vm);

    cr_assert_eq(inner_arr->marked, true, "Inner array should be marked");
    cr_assert_eq(inner1->marked, true,
                 "Inner array element 1 should be marked");
    cr_assert_eq(inner2->marked, true,
                 "Inner array element 2 should be marked");

    vm_free(vm);
}

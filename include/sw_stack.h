#include <stddef.h>
#include <stdlib.h>

typedef struct Stack {
    size_t count;
    size_t capacity;
    void **data;
} obj_stack_t;

obj_stack_t *stack_new(size_t capacity);

void stack_push(obj_stack_t *stack, void *obj);
void *stack_pop(obj_stack_t *stack);

void stack_free(obj_stack_t *stack);
void stack_remove_nulls(obj_stack_t *stack);

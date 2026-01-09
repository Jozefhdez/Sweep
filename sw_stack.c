#include "include/sw_stack.h"
#include <stdio.h>

void stack_push(obj_stack_t *stack, void *obj) {
    if (stack->count == stack->capacity) {
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->capacity * sizeof(void *));
        if (stack->data == NULL) {
            // unable to realloc, just exit :) get gud
            exit(1);
        }
    }

    stack->data[stack->count] = obj;
    stack->count++;

    return;
}

void *stack_pop(obj_stack_t *stack) {
    if (stack->count == 0) {
        return NULL;
    }

    stack->count--;
    return stack->data[stack->count];
}

void stack_free(obj_stack_t *stack) {
    if (stack == NULL) {
        return;
    }

    if (stack->data != NULL) {
        free(stack->data);
    }

    free(stack);
}

void stack_remove_nulls(obj_stack_t *stack) {
    size_t new_count = 0;

    for (size_t i = 0; i < stack->count; ++i) {
        if (stack->data[i] != NULL) {
            stack->data[new_count++] = stack->data[i];
        }
    }

    // update the count to reflect the new number of elements.
    stack->count = new_count;

    // zero out the remaining slots.
    for (size_t i = new_count; i < stack->capacity; ++i) {
        stack->data[i] = NULL;
    }
}

obj_stack_t *stack_new(size_t capacity) {
    obj_stack_t *stack = malloc(sizeof(obj_stack_t));
    if (stack == NULL) {
        return NULL;
    }

    stack->count = 0;
    stack->capacity = capacity;
    stack->data = malloc(stack->capacity * sizeof(void *));
    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }

    return stack;
}
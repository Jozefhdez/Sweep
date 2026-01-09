#include "include/sw_interpreter.h"
#include "include/sw_array.h"
#include "include/sw_obj.h"
#include "include/sw_ops.h"
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100

typedef struct {
    char *name;
    sw_obj_t *value;
} symbol_t;

static symbol_t symbols[MAX_VARS];
static int symbol_count = 0;

static sw_obj_t *get_var(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) {
            return symbols[i].value;
        }
    }
    return NULL;
}

static void set_var(const char *name, sw_obj_t *value) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) {
            symbols[i].value = value;
            return;
        }
    }
    // add new
    if (symbol_count < MAX_VARS) {
        symbols[symbol_count].name = strdup(name);
        symbols[symbol_count].value = value;
        symbol_count++;
    }
}

sw_obj_t *sw_eval(AST *ast) {
    if (!ast)
        return NULL;
    switch (ast->tag) {
    case TOKEN_INT:
        return sw_int(ast->TOKEN_INT.number);
    case TOKEN_FLOAT:
        return sw_float(ast->TOKEN_FLOAT.number);
    case TOKEN_STRING:
        return sw_string(ast->TOKEN_STRING.value);
    case TOKEN_VECTOR3: {
        sw_obj_t *x = sw_eval(ast->TOKEN_VECTOR3.x);
        sw_obj_t *y = sw_eval(ast->TOKEN_VECTOR3.y);
        sw_obj_t *z = sw_eval(ast->TOKEN_VECTOR3.z);
        if (!x || !y || !z)
            return NULL;
        return sw_vec3(x, y, z);
    }
    case TOKEN_ARRAY: {
        sw_obj_t *arr = sw_array(ast->TOKEN_ARRAY.count);
        if (!arr)
            return NULL;
        for (int i = 0; i < ast->TOKEN_ARRAY.count; i++) {
            sw_obj_t *elem = sw_eval(ast->TOKEN_ARRAY.elements[i]);
            if (!elem)
                return NULL;
            sw_array_set(arr, i, elem);
        }
        return arr;
    }
    case TOKEN_ID:
        return get_var(ast->TOKEN_ID.name);
    case TOKEN_ASSIGN: {
        sw_obj_t *val = sw_eval(ast->TOKEN_ASSIGN.expr);
        if (!val)
            return NULL;
        set_var(ast->TOKEN_ASSIGN.var->TOKEN_ID.name, val);
        return val;
    }
    case TOKEN_PLUS: {
        sw_obj_t *left = sw_eval(ast->TOKEN_PLUS.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_PLUS.right);
        if (!left || !right)
            return NULL;
        return sw_add(left, right);
    }
    case TOKEN_MINUS: {
        sw_obj_t *left = sw_eval(ast->TOKEN_PLUS.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_PLUS.right);
        if (!left || !right)
            return NULL;
        return sw_sub(left, right);
    }
    case TOKEN_MUL: {
        sw_obj_t *left = sw_eval(ast->TOKEN_PLUS.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_PLUS.right);
        if (!left || !right)
            return NULL;
        return sw_mul(left, right);
    }
    case TOKEN_DIV: {
        sw_obj_t *left = sw_eval(ast->TOKEN_PLUS.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_PLUS.right);
        if (!left || !right)
            return NULL;
        return sw_div(left, right);
    }
    default:
        return NULL;
    }
}

void free_symbols() {
    for (int i = 0; i < symbol_count; i++) {
        free(symbols[i].name);
        // note: not freeing values (sw_obj_t*) as no garbage collector yet
    }
    symbol_count = 0;
}

void get_symbol_table(sw_obj_t ***objects, int *count) {
    static sw_obj_t *symbol_objects[MAX_VARS];

    for (int i = 0; i < symbol_count; i++) {
        symbol_objects[i] = symbols[i].value;
    }

    *objects = symbol_objects;
    *count = symbol_count;
}
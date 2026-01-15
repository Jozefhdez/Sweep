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
    case TOKEN_EQ: {
        sw_obj_t *left = sw_eval(ast->TOKEN_EQ.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_EQ.right);
        if (!left || !right) return NULL;
        if (left->kind == SW_INT && right->kind == SW_INT) {
            return sw_int(left->data.v_int == right->data.v_int);
        }
        return sw_int(0);
    }
    case TOKEN_NEQ: {
        sw_obj_t *left = sw_eval(ast->TOKEN_NEQ.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_NEQ.right);
        if (!left || !right) return NULL;
        if (left->kind == SW_INT && right->kind == SW_INT) {
            return sw_int(left->data.v_int != right->data.v_int);
        }
        return sw_int(0);
    }
    case TOKEN_LT: {
        sw_obj_t *left = sw_eval(ast->TOKEN_LT.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_LT.right);
        if (!left || !right) return NULL;
        if (left->kind == SW_INT && right->kind == SW_INT) {
            return sw_int(left->data.v_int < right->data.v_int);
        }
        return sw_int(0);
    }
    case TOKEN_GT: {
        sw_obj_t *left = sw_eval(ast->TOKEN_GT.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_GT.right);
        if (!left || !right) return NULL;
        if (left->kind == SW_INT && right->kind == SW_INT) {
            return sw_int(left->data.v_int > right->data.v_int);
        }
        return sw_int(0);
    }
    case TOKEN_LE: {
        sw_obj_t *left = sw_eval(ast->TOKEN_LE.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_LE.right);
        if (!left || !right) return NULL;
        if (left->kind == SW_INT && right->kind == SW_INT) {
            return sw_int(left->data.v_int <= right->data.v_int);
        }
        return sw_int(0);
    }
    case TOKEN_GE: {
        sw_obj_t *left = sw_eval(ast->TOKEN_GE.left);
        sw_obj_t *right = sw_eval(ast->TOKEN_GE.right);
        if (!left || !right) return NULL;
        if (left->kind == SW_INT && right->kind == SW_INT) {
            return sw_int(left->data.v_int >= right->data.v_int);
        }
        return sw_int(0);
    }
    case TOKEN_AND: {
        sw_obj_t *left = sw_eval(ast->TOKEN_AND.left);
        if (!left) return NULL;
        if (left->kind == SW_INT && left->data.v_int == 0) return sw_int(0);
        sw_obj_t *right = sw_eval(ast->TOKEN_AND.right);
        if (!right) return NULL;
        if (right->kind == SW_INT && right->data.v_int == 0) return sw_int(0);
        return sw_int(1);
    }
    case TOKEN_OR: {
        sw_obj_t *left = sw_eval(ast->TOKEN_OR.left);
        if (!left) return NULL;
        if (left->kind == SW_INT && left->data.v_int != 0) return sw_int(1);
        sw_obj_t *right = sw_eval(ast->TOKEN_OR.right);
        if (!right) return NULL;
        if (right->kind == SW_INT && right->data.v_int != 0) return sw_int(1);
        return sw_int(0);
    }
    case TOKEN_NOT: {
        sw_obj_t *expr = sw_eval(ast->TOKEN_NOT.expr);
        if (!expr) return NULL;
        if (expr->kind == SW_INT && expr->data.v_int == 0) return sw_int(1);
        return sw_int(0);
    }
    case TOKEN_FN: {
        // store function
        set_var(ast->TOKEN_FN.name, (sw_obj_t *)ast); // placeholder, store AST
        return sw_int(1); // success
    }
    case TOKEN_CALL: {
        // call function
        sw_obj_t *func = sw_eval(ast->TOKEN_CALL.func);
        if (!func) return NULL;
        // evaluate args
        // call func with args
        return sw_int(0); // placeholder
    }
    case TOKEN_IF: {
        sw_obj_t *cond = sw_eval(ast->TOKEN_IF.condition);
        if (!cond) return NULL;
        if (cond) { // if true
            return sw_eval(ast->TOKEN_IF.then_branch);
        } else if (ast->TOKEN_IF.else_branch) {
            return sw_eval(ast->TOKEN_IF.else_branch);
        }
        return sw_int(0);
    }
    case TOKEN_WHILE: {
        while (1) {
            sw_obj_t *cond = sw_eval(ast->TOKEN_WHILE.condition);
            if (!cond || !cond) break;
            sw_eval(ast->TOKEN_WHILE.body);
        }
        return sw_int(0);
    }
    case TOKEN_RETURN: {
        return sw_eval(ast->TOKEN_RETURN.expr);
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
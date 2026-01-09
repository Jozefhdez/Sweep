#include "include/sw_interpreter.h"
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
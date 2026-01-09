#include "include/sw_interpreter.h"
#include "include/sw_obj.h"
#include "include/sw_ops.h"

sw_obj_t *sw_eval(AST *ast) {
    switch (ast->tag) {
    case TOKEN_INT:
        return sw_int(ast->TOKEN_INT.number);
    case TOKEN_FLOAT:
        return sw_float(ast->TOKEN_FLOAT.number);
    case TOKEN_ID:
    case TOKEN_ASSIGN:
        return NULL; // TODO: implement variables
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
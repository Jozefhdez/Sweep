#ifndef SW_PARSER_H
#define SW_PARSER_H

#include "sw_lexer.h"

typedef struct AST AST; // forward declaration

#define AST_NEW(tag, ...)                                                      \
    ast_new((TOKEN){tag, {.tag = (struct tag){__VA_ARGS__}}})

struct AST {
    token_kind_t tag;
    union {
        struct TOKEN_INT {
            int number;
        } TOKEN_INT;
        struct TOKEN_FLOAT {
            float number;
        } TOKEN_FLOAT;
        struct TOKEN_STRING {
            char *value;
        } TOKEN_STRING;
        struct TOKEN_VECTOR3 {
            AST *x;
            AST *y;
            AST *z;
        } TOKEN_VECTOR3;
        struct TOKEN_ARRAY {
            AST **elements;
            int count;
        } TOKEN_ARRAY;
        struct TOKEN_ID {
            char *name;
        } TOKEN_ID;
        struct TOKEN_ASSIGN {
            AST *var;
            AST *expr;
        } TOKEN_ASSIGN;
        struct TOKEN_PLUS {
            AST *left;
            AST *right;
        } TOKEN_PLUS;
        struct TOKEN_MINUS {
            AST *left;
            AST *right;
        } TOKEN_MINUS;
        struct TOKEN_MUL {
            AST *left;
            AST *right;
        } TOKEN_MUL;
        struct TOKEN_DIV {
            AST *left;
            AST *right;
        } TOKEN_DIV;
        struct TOKEN_FN {
            char *name;
            AST **params;
            int param_count;
            AST *body;
        } TOKEN_FN;
        struct TOKEN_CALL {
            AST *func;
            AST **args;
            int arg_count;
        } TOKEN_CALL;
        struct TOKEN_RETURN {
            AST *expr;
        } TOKEN_RETURN;
        struct TOKEN_IF {
            AST *condition;
            AST *then_branch;
            AST *else_branch;
        } TOKEN_IF;
        struct TOKEN_WHILE {
            AST *condition;
            AST *body;
        } TOKEN_WHILE;
        struct TOKEN_EQ {
            AST *left;
            AST *right;
        } TOKEN_EQ;
        struct TOKEN_NEQ {
            AST *left;
            AST *right;
        } TOKEN_NEQ;
        struct TOKEN_LT {
            AST *left;
            AST *right;
        } TOKEN_LT;
        struct TOKEN_GT {
            AST *left;
            AST *right;
        } TOKEN_GT;
        struct TOKEN_LE {
            AST *left;
            AST *right;
        } TOKEN_LE;
        struct TOKEN_GE {
            AST *left;
            AST *right;
        } TOKEN_GE;
        struct TOKEN_AND {
            AST *left;
            AST *right;
        } TOKEN_AND;
        struct TOKEN_OR {
            AST *left;
            AST *right;
        } TOKEN_OR;
        struct TOKEN_NOT {
            AST *expr;
        } TOKEN_NOT;
        struct TOKEN_LPAREN {
            AST *left;
            AST *right;
        } TOKEN_LPAREN;
        struct TOKEN_RPAREN {
            AST *left;
            AST *right;
        } TOKEN_RPAREN;
        struct TOKEN_SEMI {
            AST *left;
            AST *right;
        } TOKEN_SEMI;
    };
};

AST *ast_new(AST ast);
void ast_print(AST *ptr);
void ast_free(AST *ptr);
AST *parse(token_t *tokens);

#endif
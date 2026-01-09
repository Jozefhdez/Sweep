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

} data;

AST *ast_new(AST ast);
void ast_print(AST *ptr);
void ast_free(AST *ptr);
AST *parse(token_t *tokens);

#endif
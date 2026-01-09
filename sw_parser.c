#include "include/sw_parser.h"
#include "include/sw_lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static token_t *tokens;
static int current = 0;

static AST *parse_expr();
static AST *parse_add();
static AST *parse_term();
static AST *parse_factor();
static int match(token_kind_t kind);

AST *ast_new(AST ast) {
    AST *ptr = malloc(sizeof(AST));
    if (ptr)
        *ptr = ast;
    return ptr;
}

void ast_print(AST *ptr) {
    if (!ptr) {
        printf("NULL");
        return;
    }
    AST ast = *ptr;
    switch (ast.tag) {
    case TOKEN_INT: {
        printf("%d", ast.TOKEN_INT.number);
        break;
    }
    case TOKEN_FLOAT: {
        printf("%f", ast.TOKEN_FLOAT.number);
        break;
    }
    case TOKEN_STRING: {
        printf("\"%s\"", ast.TOKEN_STRING.value);
        break;
    }
    case TOKEN_VECTOR3: {
        printf("vec3(");
        ast_print(ast.TOKEN_VECTOR3.x);
        printf(", ");
        ast_print(ast.TOKEN_VECTOR3.y);
        printf(", ");
        ast_print(ast.TOKEN_VECTOR3.z);
        printf(")");
        break;
    }
    case TOKEN_ID: {
        printf("%s", ast.TOKEN_ID.name);
        break;
    }
    case TOKEN_PLUS: {
        printf("(");
        ast_print(ast.TOKEN_PLUS.left);
        printf(" + ");
        ast_print(ast.TOKEN_PLUS.right);
        printf(")");
        break;
    }
    case TOKEN_MINUS: {
        printf("(");
        ast_print(ast.TOKEN_MINUS.left);
        printf(" - ");
        ast_print(ast.TOKEN_MINUS.right);
        printf(")");
        break;
    }
    case TOKEN_MUL: {
        printf("(");
        ast_print(ast.TOKEN_MUL.left);
        printf(" * ");
        ast_print(ast.TOKEN_MUL.right);
        printf(")");
        break;
    }
    case TOKEN_DIV: {
        printf("(");
        ast_print(ast.TOKEN_DIV.left);
        printf(" / ");
        ast_print(ast.TOKEN_DIV.right);
        printf(")");
        break;
    }
    case TOKEN_ASSIGN: {
        ast_print(ast.TOKEN_ASSIGN.var);
        printf(" := ");
        ast_print(ast.TOKEN_ASSIGN.expr);
        break;
    }
    default: {
        printf("UNKNOWN");
        break;
    }
    }
}

void ast_free(AST *ast) {
    if (!ast)
        return; // NULL safety

    switch (ast->tag) {
    case TOKEN_INT:
    case TOKEN_FLOAT:
        free(ast);
        break;
    case TOKEN_STRING:
        free(ast->TOKEN_STRING.value);
        free(ast);
        break;
    case TOKEN_VECTOR3:
        ast_free(ast->TOKEN_VECTOR3.x);
        ast_free(ast->TOKEN_VECTOR3.y);
        ast_free(ast->TOKEN_VECTOR3.z);
        free(ast);
        break;
    case TOKEN_ID:
        free(ast->TOKEN_ID.name);
        free(ast);
        break;
    case TOKEN_ASSIGN:
        // free recursively
        ast_free(ast->TOKEN_ASSIGN.var);
        ast_free(ast->TOKEN_ASSIGN.expr);
        free(ast);
        break;
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_MUL:
    case TOKEN_DIV:
        // free recursively
        ast_free(ast->TOKEN_PLUS.left);
        ast_free(ast->TOKEN_PLUS.right);
        free(ast);
        break;
    default:
        free(ast);
        break;
    }
}

/*
    Initializes global tokens array and calls parse_expr() to start parsing
*/
AST *parse(token_t *toks) {
    tokens = toks;
    current = 0;
    return parse_expr();
}

/*
    Parses expressions, including assignments with lowest precedence
*/
static AST *parse_expr() {
    AST *left = parse_add();
    if (!left)
        return NULL;
    if (match(TOKEN_ASSIGN)) {
        AST *right = parse_expr();
        if (!right)
            return NULL;
        AST *new_left =
            ast_new((AST){TOKEN_ASSIGN, {.TOKEN_ASSIGN = {left, right}}});
        if (!new_left)
            return NULL;
        left = new_left;
    }
    return left;
}

/*
    Parses low precedence operators (+ and -), reads a term and repeats while it
   finds + or
   -, creating recursive binary nodes.

   Ej. for 5 + 3 - 2 it creates ((5 + 3) - 2)
*/
static AST *parse_add() {
    AST *left = parse_term();
    if (!left)
        return NULL;
    while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        token_kind_t op = tokens[current - 1].kind; // matching op
        AST *right = parse_term();
        if (!right)
            return NULL;
        AST *new_left = ast_new(
            (AST){op, {.TOKEN_PLUS = {left, right}}}); // adjust for correct op
        if (!new_left)
            return NULL;
        left = new_left;
    }
    return left;
}

/*
   Parses terms with high precedence (* and /), it reads a factor and repeats
   while it finds * or /. Ej. for 3 * 2 + 1, first it parses 3 * 2, then
   parse_add() adds + 1
*/

static AST *parse_term() {
    AST *left = parse_factor();
    if (!left)
        return NULL;
    while (match(TOKEN_MUL) || match(TOKEN_DIV)) {
        token_kind_t op = tokens[current - 1].kind;
        AST *right = parse_factor();
        if (!right)
            return NULL;
        AST *new_left = ast_new((AST){op, {.TOKEN_MUL = {left, right}}});
        if (!new_left)
            return NULL;
        left = new_left;
    }
    return left;
}

/*
    Parses most basic factors, ints, floats, ids or parenthesized expressions
    Ej. for 6, creates a TOKEN_INT node.
    For (2 + 3), calls recursively parse_expr()
*/
static AST *parse_factor() {
    if (match(TOKEN_INT)) {
        return ast_new((AST){
            TOKEN_INT, {.TOKEN_INT = {atoi(tokens[current - 1].lexeme)}}});
    }
    if (match(TOKEN_FLOAT)) {
        return ast_new((AST){
            TOKEN_FLOAT, {.TOKEN_FLOAT = {atof(tokens[current - 1].lexeme)}}});
    }
    if (match(TOKEN_STRING)) {
        return ast_new(
            (AST){TOKEN_STRING,
                  {.TOKEN_STRING = {strdup(tokens[current - 1].lexeme)}}});
    }
    if (match(TOKEN_ID)) {
        return ast_new((AST){
            TOKEN_ID, {.TOKEN_ID = {strdup(tokens[current - 1].lexeme)}}});
    }
    if (match(TOKEN_LPAREN)) {
        AST *first = parse_expr();
        if (!first) return NULL;
        if (match(TOKEN_COMMA)) {
            // vector literal (1,2,3)
            AST *second = parse_expr();
            if (!second || !match(TOKEN_COMMA)) return NULL;
            AST *third = parse_expr();
            if (!third || !match(TOKEN_RPAREN)) return NULL;
            return ast_new((AST){TOKEN_VECTOR3, {.TOKEN_VECTOR3 = {first, second, third}}});
        } else if (match(TOKEN_RPAREN)) {
            // parenthesized expression
            return first;
        } else {
            return NULL;
        }
    }
    // error
    return NULL;
}

/*
    verifies if the current token matches with kind, if so it advances and
   return true, otherwise it returns 0 without moving.
*/
static int match(token_kind_t kind) {
    if (tokens[current].kind == kind) {
        current++;
        return 1;
    }
    return 0;
}

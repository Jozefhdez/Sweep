#include "include/sw_parser.h"
#include "include/sw_lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static token_t *tokens;
static int current = 0;

static AST *parse_statement();
static AST *parse_comparison();
static AST *parse_logical();
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
    case TOKEN_ARRAY: {
        printf("[");
        for (int i = 0; i < ast.TOKEN_ARRAY.count; i++) {
            ast_print(ast.TOKEN_ARRAY.elements[i]);
            if (i < ast.TOKEN_ARRAY.count - 1)
                printf(", ");
        }
        printf("]");
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
    case TOKEN_ARRAY:
        for (int i = 0; i < ast->TOKEN_ARRAY.count; i++) {
            ast_free(ast->TOKEN_ARRAY.elements[i]);
        }
        free(ast->TOKEN_ARRAY.elements);
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
    return parse_statement();
}

static AST *parse_statement() {
    if (match(TOKEN_FN)) {
        // function definition
        if (tokens[current].kind != TOKEN_ID) return NULL;
        char *name = strdup(tokens[current].lexeme);
        current++;
        if (!match(TOKEN_LPAREN)) return NULL;
        // parse parameters
        AST **params = NULL;
        int param_count = 0;
        while (tokens[current].kind == TOKEN_ID) {
            params = realloc(params, sizeof(AST *) * (param_count + 1));
            params[param_count++] = ast_new((AST){TOKEN_ID, {.TOKEN_ID = {strdup(tokens[current].lexeme)}}});
            current++;
            if (!match(TOKEN_COMMA)) break;
        }
        if (!match(TOKEN_RPAREN)) return NULL;
        if (!match(TOKEN_LBRACE)) return NULL;
        AST *body = parse_statement(); // for now, simple body
        if (!match(TOKEN_RBRACE)) return NULL;
        return ast_new((AST){TOKEN_FN, {.TOKEN_FN = {name, params, param_count, body}}});
    } else if (match(TOKEN_IF)) {
        if (!match(TOKEN_LPAREN)) return NULL;
        AST *condition = parse_expr();
        if (!match(TOKEN_RPAREN)) return NULL;
        if (!match(TOKEN_LBRACE)) return NULL;
        AST *then_branch = parse_statement();
        if (!match(TOKEN_RBRACE)) return NULL;
        AST *else_branch = NULL;
        if (match(TOKEN_ELSE)) {
            if (!match(TOKEN_LBRACE)) return NULL;
            else_branch = parse_statement();
            if (!match(TOKEN_RBRACE)) return NULL;
        }
        return ast_new((AST){TOKEN_IF, {.TOKEN_IF = {condition, then_branch, else_branch}}});
    } else if (match(TOKEN_WHILE)) {
        if (!match(TOKEN_LPAREN)) return NULL;
        AST *condition = parse_expr();
        if (!match(TOKEN_RPAREN)) return NULL;
        if (!match(TOKEN_LBRACE)) return NULL;
        AST *body = parse_statement();
        if (!match(TOKEN_RBRACE)) return NULL;
        return ast_new((AST){TOKEN_WHILE, {.TOKEN_WHILE = {condition, body}}});
    } else if (match(TOKEN_RETURN)) {
        AST *expr = parse_expr();
        return ast_new((AST){TOKEN_RETURN, {.TOKEN_RETURN = {expr}}});
    } else {
        return parse_expr();
    }
}

/*
    Parses expressions, including assignments with lowest precedence
*/
static AST *parse_expr() {
    AST *left = parse_logical();
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
    Parses logical operators (&& and ||)
*/
static AST *parse_logical() {
    AST *left = parse_comparison();
    if (!left) return NULL;
    while (match(TOKEN_AND) || match(TOKEN_OR)) {
        token_kind_t op = tokens[current - 1].kind;
        AST *right = parse_comparison();
        if (!right) return NULL;
        left = ast_new((AST){op, {.TOKEN_AND = {left, right}}}); // reuse TOKEN_AND for both
    }
    return left;
}

/*
    Parses comparison operators (== != < > <= >=)
*/
static AST *parse_comparison() {
    AST *left = parse_add();
    if (!left) return NULL;
    if (match(TOKEN_EQ) || match(TOKEN_NEQ) || match(TOKEN_LT) || match(TOKEN_GT) || match(TOKEN_LE) || match(TOKEN_GE)) {
        token_kind_t op = tokens[current - 1].kind;
        AST *right = parse_add();
        if (!right) return NULL;
        left = ast_new((AST){op, {.TOKEN_EQ = {left, right}}}); // reuse TOKEN_EQ for all
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
    if (match(TOKEN_NOT)) {
        AST *expr = parse_factor();
        if (!expr) return NULL;
        return ast_new((AST){TOKEN_NOT, {.TOKEN_NOT = {expr}}});
    }
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
        AST *id = ast_new((AST){
            TOKEN_ID, {.TOKEN_ID = {strdup(tokens[current - 1].lexeme)}}});
        if (match(TOKEN_LPAREN)) {
            // function call
            int capacity = 4;
            AST **args = malloc(sizeof(AST *) * capacity);
            int count = 0;
            if (!match(TOKEN_RPAREN)) {
                do {
                    if (count >= capacity) {
                        capacity *= 2;
                        args = realloc(args, sizeof(AST *) * capacity);
                    }
                    AST *arg = parse_expr();
                    if (!arg) {
                        for (int i = 0; i < count; i++) ast_free(args[i]);
                        free(args);
                        ast_free(id);
                        return NULL;
                    }
                    args[count++] = arg;
                } while (match(TOKEN_COMMA));
                if (!match(TOKEN_RPAREN)) {
                    for (int i = 0; i < count; i++) ast_free(args[i]);
                    free(args);
                    ast_free(id);
                    return NULL;
                }
            }
            return ast_new((AST){TOKEN_CALL, {.TOKEN_CALL = {id, args, count}}});
        }
        return id;
    }
    if (match(TOKEN_LBRACKET)) {
        // Array literal [1, 2, 3]
        int capacity = 4;
        AST **elements = malloc(sizeof(AST *) * capacity);
        int count = 0;

        if (!match(TOKEN_RBRACKET)) {
            do {
                if (count >= capacity) {
                    capacity *= 2;
                    elements = realloc(elements, sizeof(AST *) * capacity);
                }
                AST *elem = parse_expr();
                if (!elem) {
                    for (int i = 0; i < count; i++)
                        ast_free(elements[i]);
                    free(elements);
                    return NULL;
                }
                elements[count++] = elem;
            } while (match(TOKEN_COMMA));

            if (!match(TOKEN_RBRACKET)) {
                // error: missing ]
                for (int i = 0; i < count; i++)
                    ast_free(elements[i]);
                free(elements);
                return NULL;
            }
        }

        return ast_new((AST){TOKEN_ARRAY, {.TOKEN_ARRAY = {elements, count}}});
    }
    if (match(TOKEN_LPAREN)) {
        AST *first = parse_expr();
        if (!first)
            return NULL;
        if (match(TOKEN_COMMA)) {
            // vector literal (1,2,3)
            AST *second = parse_expr();
            if (!second || !match(TOKEN_COMMA))
                return NULL;
            AST *third = parse_expr();
            if (!third || !match(TOKEN_RPAREN))
                return NULL;
            return ast_new((AST){TOKEN_VECTOR3,
                                 {.TOKEN_VECTOR3 = {first, second, third}}});
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

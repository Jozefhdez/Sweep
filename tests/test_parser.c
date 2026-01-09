#include "../include/sw_lexer.h"
#include "../include/sw_parser.h"
#include <criterion/criterion.h>

Test(parser, parse_integer) {
    token_t *tokens = sw_lex("42");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_INT, "AST tag should be TOKEN_INT");
    cr_assert_eq(ast->TOKEN_INT.number, 42, "Integer value should be 42");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, parse_float) {
    token_t *tokens = sw_lex("3.14");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_FLOAT, "AST tag should be TOKEN_FLOAT");
    cr_assert_float_eq(ast->TOKEN_FLOAT.number, 3.14f, 0.01f,
                       "Float value should be 3.14");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, parse_string) {
    token_t *tokens = sw_lex("\"Hello\"");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_STRING, "AST tag should be TOKEN_STRING");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, parse_addition) {
    token_t *tokens = sw_lex("5 + 3");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_PLUS, "AST tag should be TOKEN_PLUS");
    cr_assert_not_null(ast->TOKEN_PLUS.left, "Left operand should not be NULL");
    cr_assert_not_null(ast->TOKEN_PLUS.right,
                       "Right operand should not be NULL");
    cr_assert_eq(ast->TOKEN_PLUS.left->tag, TOKEN_INT, "Left should be INT");
    cr_assert_eq(ast->TOKEN_PLUS.right->tag, TOKEN_INT, "Right should be INT");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, operator_precedence) {
    token_t *tokens = sw_lex("2 + 3 * 4");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_PLUS, "Root should be addition");
    cr_assert_eq(ast->TOKEN_PLUS.left->tag, TOKEN_INT, "Left should be 2");
    cr_assert_eq(ast->TOKEN_PLUS.right->tag, TOKEN_MUL,
                 "Right should be multiplication (3*4)");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, parse_vector) {
    token_t *tokens = sw_lex("(1, 2, 3)");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_VECTOR3, "AST tag should be TOKEN_VECTOR3");
    cr_assert_not_null(ast->TOKEN_VECTOR3.x, "X component should not be NULL");
    cr_assert_not_null(ast->TOKEN_VECTOR3.y, "Y component should not be NULL");
    cr_assert_not_null(ast->TOKEN_VECTOR3.z, "Z component should not be NULL");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, parse_array) {
    token_t *tokens = sw_lex("[1, 2, 3]");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_ARRAY, "AST tag should be TOKEN_ARRAY");
    cr_assert_eq(ast->TOKEN_ARRAY.count, 3, "Array should have 3 elements");
    cr_assert_not_null(ast->TOKEN_ARRAY.elements,
                       "Elements array should not be NULL");

    ast_free(ast);
    free_tokens(tokens);
}

Test(parser, parse_assignment) {
    token_t *tokens = sw_lex("x := 42");
    AST *ast = parse(tokens);

    cr_assert_not_null(ast, "AST should not be NULL");
    cr_assert_eq(ast->tag, TOKEN_ASSIGN, "AST tag should be TOKEN_ASSIGN");
    cr_assert_not_null(ast->TOKEN_ASSIGN.var, "Variable should not be NULL");
    cr_assert_not_null(ast->TOKEN_ASSIGN.expr, "Expression should not be NULL");
    cr_assert_eq(ast->TOKEN_ASSIGN.var->tag, TOKEN_ID,
                 "Variable should be identifier");

    ast_free(ast);
    free_tokens(tokens);
}

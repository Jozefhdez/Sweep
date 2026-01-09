#include "../include/sw_lexer.h"
#include <criterion/criterion.h>

Test(lexer, basic_tokens) {
    token_t *tokens = sw_lex("42 + 3.14");

    cr_assert_eq(tokens[0].kind, TOKEN_INT, "First token should be INT");
    cr_assert_str_eq(tokens[0].lexeme, "42", "First lexeme should be '42'");

    cr_assert_eq(tokens[1].kind, TOKEN_PLUS, "Second token should be PLUS");
    cr_assert_str_eq(tokens[1].lexeme, "+", "Second lexeme should be '+'");

    cr_assert_eq(tokens[2].kind, TOKEN_FLOAT, "Third token should be FLOAT");
    cr_assert_str_eq(tokens[2].lexeme, "3.14", "Third lexeme should be '3.14'");

    cr_assert_eq(tokens[3].kind, TOKEN_EOF, "Fourth token should be EOF");

    free_tokens(tokens);
}

Test(lexer, string_literal) {
    token_t *tokens = sw_lex("\"Hello World\"");

    cr_assert_eq(tokens[0].kind, TOKEN_STRING, "Token should be STRING");
    cr_assert_str_eq(tokens[0].lexeme, "Hello World",
                     "String content should match");
    cr_assert_eq(tokens[1].kind, TOKEN_EOF, "Should end with EOF");

    free_tokens(tokens);
}

Test(lexer, vector_syntax) {
    token_t *tokens = sw_lex("(1, 2, 3)");

    cr_assert_eq(tokens[0].kind, TOKEN_LPAREN);
    cr_assert_eq(tokens[1].kind, TOKEN_INT);
    cr_assert_eq(tokens[2].kind, TOKEN_COMMA);
    cr_assert_eq(tokens[3].kind, TOKEN_INT);
    cr_assert_eq(tokens[4].kind, TOKEN_COMMA);
    cr_assert_eq(tokens[5].kind, TOKEN_INT);
    cr_assert_eq(tokens[6].kind, TOKEN_RPAREN);
    cr_assert_eq(tokens[7].kind, TOKEN_EOF);

    free_tokens(tokens);
}

Test(lexer, array_syntax) {
    token_t *tokens = sw_lex("[1, 2, 3]");

    cr_assert_eq(tokens[0].kind, TOKEN_LBRACKET);
    cr_assert_eq(tokens[1].kind, TOKEN_INT);
    cr_assert_eq(tokens[2].kind, TOKEN_COMMA);
    cr_assert_eq(tokens[3].kind, TOKEN_INT);
    cr_assert_eq(tokens[4].kind, TOKEN_COMMA);
    cr_assert_eq(tokens[5].kind, TOKEN_INT);
    cr_assert_eq(tokens[6].kind, TOKEN_RBRACKET);
    cr_assert_eq(tokens[7].kind, TOKEN_EOF);

    free_tokens(tokens);
}

Test(lexer, assignment) {
    token_t *tokens = sw_lex("x := 42");

    cr_assert_eq(tokens[0].kind, TOKEN_ID);
    cr_assert_str_eq(tokens[0].lexeme, "x");

    cr_assert_eq(tokens[1].kind, TOKEN_ASSIGN);
    cr_assert_str_eq(tokens[1].lexeme, ":=");

    cr_assert_eq(tokens[2].kind, TOKEN_INT);
    cr_assert_str_eq(tokens[2].lexeme, "42");

    cr_assert_eq(tokens[3].kind, TOKEN_EOF);

    free_tokens(tokens);
}

Test(lexer, all_operators) {
    token_t *tokens = sw_lex("5 + 3 - 2 * 4 / 2");

    cr_assert_eq(tokens[0].kind, TOKEN_INT);
    cr_assert_eq(tokens[1].kind, TOKEN_PLUS);
    cr_assert_eq(tokens[2].kind, TOKEN_INT);
    cr_assert_eq(tokens[3].kind, TOKEN_MINUS);
    cr_assert_eq(tokens[4].kind, TOKEN_INT);
    cr_assert_eq(tokens[5].kind, TOKEN_MUL);
    cr_assert_eq(tokens[6].kind, TOKEN_INT);
    cr_assert_eq(tokens[7].kind, TOKEN_DIV);
    cr_assert_eq(tokens[8].kind, TOKEN_INT);
    cr_assert_eq(tokens[9].kind, TOKEN_EOF);

    free_tokens(tokens);
}

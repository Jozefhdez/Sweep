#include "sw_array.h"
#include "sw_interpreter.h"
#include "sw_lexer.h"
#include "sw_obj.h"
#include "sw_ops.h"
#include "sw_parser.h"
#include <stdio.h>

int main() {
    const char *code = "x := \"hello\" + \" world!\"";

    printf("Testing Sweep expression: %s\n\n", code);

    token_t *tokens = sw_lex(code);
    printf("Tokens:\n");
    for (int i = 0; tokens[i].kind != TOKEN_EOF; i++) {
        printf("%s\n", tokens[i].lexeme);
    }
    printf("\n");

    AST *ast = parse(tokens);
    if (ast) {
        printf("AST: ");
        ast_print(ast);
        printf("\n\n");

        sw_obj_t *result = sw_eval(ast);
        if (result) {
            printf("Result: ");
            sw_print(result);
            printf("\n");
        } else {
            printf("Evaluation error\n");
        }

        ast_free(ast);
    } else {
        printf("Parse error\n");
    }

    free_tokens(tokens);
    return 0;
}

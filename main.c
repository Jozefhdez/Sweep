#include "sw_array.h"
#include "sw_interpreter.h"
#include "sw_lexer.h"
#include "sw_obj.h"
#include "sw_ops.h"
#include "sw_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Test multiple Sweep statements
    const char *code = "x := (1,2,3) + (4,5,6)\ny := x * 2\nz := y - (1,1,1)";

    // Split code by newlines for multiple statements
    char *code_copy = strdup(code);
    char *line = strtok(code_copy, "\n");
    while (line != NULL) {
        printf("Statement: %s\n", line);

        token_t *tokens = sw_lex(line);
        printf("Tokens:\n");
        for (int i = 0; tokens[i].kind != TOKEN_EOF; i++) {
            printf("  %s\n", tokens[i].lexeme);
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
        printf("\n");
        line = strtok(NULL, "\n");
    }
    free(code_copy);
    return 0;
}

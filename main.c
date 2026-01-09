#include "sw_array.h"
#include "sw_input.h"
#include "sw_interpreter.h"
#include "sw_lexer.h"
#include "sw_obj.h"
#include "sw_ops.h"
#include "sw_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_prompt() {
    printf("Sweep > ");
}

// remember to delete unnecesarry prints and only left the result

int main() {
    InputBuffer *input_buffer = new_input_buffer();

    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            MetaCommandType type = get_meta_command_type(input_buffer->buffer);
            if (type == COMMAND_EXIT) {
                close_input_buffer(input_buffer);
                free_symbols();
                return 0;
            } else {
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
            }
            continue;
        }

        token_t *tokens = sw_lex(input_buffer->buffer);

        AST *ast = parse(tokens);
        if (ast) {
            sw_obj_t *result = sw_eval(ast);
            if (result) {
                printf("\n");
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
    }
}
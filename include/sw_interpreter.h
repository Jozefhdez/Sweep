#ifndef SW_INTERPRETER_H
#define SW_INTERPRETER_H
#include "sw_parser.h"
#include "sw_types.h"

sw_obj_t *sw_eval(AST *ast);
void free_symbols();
void get_symbol_table(sw_obj_t ***objects, int *count);

#endif
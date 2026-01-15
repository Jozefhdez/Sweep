#ifndef SW_LEXER_H
#define SW_LEXER_H

typedef enum {
    TOKEN_EOF,      // end of input
    TOKEN_INT,      // 67
    TOKEN_FLOAT,    // 6.7
    TOKEN_STRING,   // "hello"
    TOKEN_VECTOR3,  // (1, 2, 3)
    TOKEN_ARRAY,    // [1, 2, 3]
    TOKEN_ID,       // ej. x, var
    TOKEN_ASSIGN,   // :=
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_MUL,      // *
    TOKEN_DIV,      // /
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_LBRACKET, // [
    TOKEN_RBRACKET, // ]
    TOKEN_COMMA,    // ,
    TOKEN_SEMI,     // ;
    TOKEN_LBRACE,   // {
    TOKEN_RBRACE,   // }
    TOKEN_FN,       // fn
    TOKEN_RETURN,   // return
    TOKEN_IF,       // if
    TOKEN_ELSE,     // else
    TOKEN_WHILE,    // while
    TOKEN_EQ,       // ==
    TOKEN_NEQ,      // !=
    TOKEN_LT,       // <
    TOKEN_GT,       // >
    TOKEN_LE,       // <=
    TOKEN_GE,       // >=
    TOKEN_AND,      // &&
    TOKEN_OR,       // ||
    TOKEN_NOT,      // !
    TOKEN_CALL,     // function call
    TOKEN_ERROR     // lexical error
} token_kind_t;

typedef struct {
    token_kind_t kind;
    char *lexeme; // token text
    int line;     // line number
} token_t;

token_t *sw_lex(const char *source);
void free_tokens(token_t *tokens);

#endif
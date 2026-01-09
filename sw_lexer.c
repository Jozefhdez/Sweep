#include "sw_lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}
static int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

static token_t make_token(token_kind_t kind, const char *start, int length,
                          int line) {
    token_t token;
    token.kind = kind;
    token.lexeme = malloc(length + 1);
    strncpy(token.lexeme, start, length);
    token.lexeme[length] = '\0';
    token.line = line;
    return token;
}

token_t *sw_lex(const char *source) {
    int capacity = 8;
    token_t *tokens = malloc(sizeof(token_t) * capacity);

    int count = 0;
    int start = 0;
    int current = 0;
    int line = 1;

    while (source[current] != '\0') {
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, sizeof(token_t) * capacity);
        }
        start = current;
        char c = source[current++];

        switch (c) {
        case ' ':
        case '\t':
        case '\r':
            break;
        case '\n':
            line++;
            break;
        case '+':
            tokens[count++] = make_token(TOKEN_PLUS, &source[start], 1, line);
            break;
        case '-':
            tokens[count++] = make_token(TOKEN_MINUS, &source[start], 1, line);
            break;
        case '*':
            tokens[count++] = make_token(TOKEN_MUL, &source[start], 1, line);
            break;
        case '/':
            tokens[count++] = make_token(TOKEN_DIV, &source[start], 1, line);
            break;
        case '(':
            tokens[count++] = make_token(TOKEN_LPAREN, &source[start], 1, line);
            break;
        case ')':
            tokens[count++] = make_token(TOKEN_RPAREN, &source[start], 1, line);
            break;
        case ';':
            tokens[count++] = make_token(TOKEN_SEMI, &source[start], 1, line);
            break;
        case '"':
            start = current;
            while (source[current] != '"' && source[current] != '\0') {
                current++;
            }
            int length = current - start;
            if (source[current] == '"') {
                current++;
            }
            tokens[count++] =
                make_token(TOKEN_STRING, &source[start], length, line);
            break;
        default:
            if (is_digit(c)) {
                while (is_digit(source[current])) {
                    current++;
                }
                if (source[current] == '.') {
                    current++;
                    while (is_digit(source[current])) {
                        current++;
                    }
                    tokens[count++] = make_token(TOKEN_FLOAT, &source[start],
                                                 current - start, line);
                } else {
                    tokens[count++] = make_token(TOKEN_INT, &source[start],
                                                 current - start, line);
                }
            } else if (is_alpha(c)) {
                while (is_alnum(source[current])) {
                    current++;
                }
                tokens[count++] =
                    make_token(TOKEN_ID, &source[start], current - start, line);
            } else if (c == ':' && source[current] == '=') {
                current++;
                tokens[count++] =
                    make_token(TOKEN_ASSIGN, &source[start], 2, line);
            } else {
                tokens[count++] =
                    make_token(TOKEN_ERROR, &source[start], 1, line);
            }
            break;
        }
    }

    // resize to exact size
    tokens = realloc(tokens, sizeof(token_t) * (count + 1));
    tokens[count] = make_token(TOKEN_EOF, "", 0, line);
    return tokens;
}

void free_tokens(token_t *tokens) {
    for (int i = 0; tokens[i].kind != TOKEN_EOF; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);
}
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
static token_kind_t check_keyword(const char *start, int length) {
    if (length == 2 && strncmp(start, "fn", 2) == 0)
        return TOKEN_FN;
    if (length == 2 && strncmp(start, "if", 2) == 0)
        return TOKEN_IF;
    if (length == 4 && strncmp(start, "else", 4) == 0)
        return TOKEN_ELSE;
    if (length == 5 && strncmp(start, "while", 5) == 0)
        return TOKEN_WHILE;
    if (length == 6 && strncmp(start, "return", 6) == 0)
        return TOKEN_RETURN;
    return TOKEN_ID;
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
        case '[':
            tokens[count++] =
                make_token(TOKEN_LBRACKET, &source[start], 1, line);
            break;
        case ']':
            tokens[count++] =
                make_token(TOKEN_RBRACKET, &source[start], 1, line);
            break;
        case ',':
            tokens[count++] = make_token(TOKEN_COMMA, &source[start], 1, line);
            break;
        case ';':
            tokens[count++] = make_token(TOKEN_SEMI, &source[start], 1, line);
            break;
        case '{':
            tokens[count++] = make_token(TOKEN_LBRACE, &source[start], 1, line);
            break;
        case '}':
            tokens[count++] = make_token(TOKEN_RBRACE, &source[start], 1, line);
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
                int length = current - start;
                token_kind_t kind = check_keyword(&source[start], length);
                tokens[count++] =
                    make_token(kind, &source[start], length, line);
            } else if (c == ':' && source[current] == '=') {
                current++;
                tokens[count++] =
                    make_token(TOKEN_ASSIGN, &source[start], 2, line);
            } else if (c == '=' && source[current] == '=') {
                current++;
                tokens[count++] = make_token(TOKEN_EQ, &source[start], 2, line);
            } else if (c == '!' && source[current] == '=') {
                current++;
                tokens[count++] =
                    make_token(TOKEN_NEQ, &source[start], 2, line);
            } else if (c == '<' && source[current] == '=') {
                current++;
                tokens[count++] = make_token(TOKEN_LE, &source[start], 2, line);
            } else if (c == '>' && source[current] == '=') {
                current++;
                tokens[count++] = make_token(TOKEN_GE, &source[start], 2, line);
            } else if (c == '&' && source[current] == '&') {
                current++;
                tokens[count++] =
                    make_token(TOKEN_AND, &source[start], 2, line);
            } else if (c == '|' && source[current] == '|') {
                current++;
                tokens[count++] = make_token(TOKEN_OR, &source[start], 2, line);
            } else if (c == '<') {
                tokens[count++] = make_token(TOKEN_LT, &source[start], 1, line);
            } else if (c == '>') {
                tokens[count++] = make_token(TOKEN_GT, &source[start], 1, line);
            } else if (c == '!') {
                tokens[count++] =
                    make_token(TOKEN_NOT, &source[start], 1, line);
            } else {
                break;
            }
        }
    }

    // resize to exact size
    tokens = realloc(tokens, sizeof(token_t) * (count + 1));
    tokens[count] = make_token(TOKEN_EOF, "", 0, line);
    return tokens;
}
void free_tokens(token_t *tokens) {
    for (int i = 0;; i++) {
        token_kind_t kind = tokens[i].kind;
        free(tokens[i].lexeme);
        if (kind == TOKEN_EOF)
            break;
    }
    free(tokens);
}
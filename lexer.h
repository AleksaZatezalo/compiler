#ifndef LEXER_H
#define LEXER_H

// Token types
typedef enum {
    TOKEN_INT,
    TOKEN_MAIN,
    TOKEN_VOID,
    TOKEN_RETURN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_NUMBER,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

// Function declarations
Token* lex(char **input);
Token** tokenize(char *source, int *token_count);
void print_tokens(Token **tokens, int count);
void free_tokens(Token **tokens, int count);

#endif // LEXER_H

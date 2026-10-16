#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

// Simple lexer function
Token* lex(char **input) {
    // Skip whitespace
    while (**input && isspace(**input)) {
        (*input)++;
    }
    
    // Check for end of input
    if (**input == '\0') {
        Token *token = malloc(sizeof(Token));
        token->type = TOKEN_EOF;
        token->value = NULL;
        return token;
    }
    
    Token *token = malloc(sizeof(Token));
    
    // Single character tokens
    if (**input == '(') {
        token->type = TOKEN_LPAREN;
        token->value = strdup("(");
        (*input)++;
        return token;
    }
    if (**input == ')') {
        token->type = TOKEN_RPAREN;
        token->value = strdup(")");
        (*input)++;
        return token;
    }
    if (**input == '{') {
        token->type = TOKEN_LBRACE;
        token->value = strdup("{");
        (*input)++;
        return token;
    }
    if (**input == '}') {
        token->type = TOKEN_RBRACE;
        token->value = strdup("}");
        (*input)++;
        return token;
    }
    if (**input == ';') {
        token->type = TOKEN_SEMICOLON;
        token->value = strdup(";");
        (*input)++;
        return token;
    }
    
    // Numbers
    if (isdigit(**input)) {
        int start = 0;
        char buffer[256];
        while (isdigit(**input)) {
            buffer[start++] = **input;
            (*input)++;
        }
        buffer[start] = '\0';
        token->type = TOKEN_NUMBER;
        token->value = strdup(buffer);
        return token;
    }
    
    // Keywords and identifiers
    if (isalpha(**input)) {
        int start = 0;
        char buffer[256];
        while (isalpha(**input)) {
            buffer[start++] = **input;
            (*input)++;
        }
        buffer[start] = '\0';
        
        // Check keywords
        if (strcmp(buffer, "int") == 0) {
            token->type = TOKEN_INT;
        } else if (strcmp(buffer, "main") == 0) {
            token->type = TOKEN_MAIN;
        } else if (strcmp(buffer, "void") == 0) {
            token->type = TOKEN_VOID;
        } else if (strcmp(buffer, "return") == 0) {
            token->type = TOKEN_RETURN;
        }
        token->value = strdup(buffer);
        return token;
    }
    
    // If we get here, unrecognized character
    fprintf(stderr, "Error: unrecognized character '%c'\n", **input);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source.c>\n", argv[0]);
        return 1;
    }
    
    // Read the source file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", argv[1]);
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *source = malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    
    // Lex the source
    char *input = source;
    Token *token;
    
    while ((token = lex(&input))->type != TOKEN_EOF) {
        printf("Token: type=%d, value='%s'\n", token->type, token->value);
        free(token->value);
        free(token);
    }
    
    free(token);
    free(source);
    
    return 0;
}

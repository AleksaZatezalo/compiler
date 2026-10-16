#ifndef LEXER_H
#define LEXER_H

#define _POSIX_C_SOURCE 200809L

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

// Lexer function - converts input string to a single token
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

// Helper function to tokenize entire source into array
Token** tokenize(char *source, int *token_count) {
    char *input = source;
    Token *token;
    int count = 0;
    int capacity = 100;
    Token **tokens = malloc(sizeof(Token*) * capacity);
    
    while (1) {
        token = lex(&input);
        tokens[count++] = token;
        
        if (token->type == TOKEN_EOF) {
            break;
        }
        
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, sizeof(Token*) * capacity);
        }
    }
    
    *token_count = count;
    return tokens;
}

// Helper function to print tokens (for debugging)
void print_tokens(Token **tokens, int count) {
    printf("=== Tokens ===\n");
    for (int i = 0; i < count; i++) {
        printf("Token: type=%d, value='%s'\n", 
               tokens[i]->type, 
               tokens[i]->value ? tokens[i]->value : "NULL");
    }
}

// Helper function to free tokens
void free_tokens(Token **tokens, int count) {
    for (int i = 0; i < count; i++) {
        if (tokens[i]->value) {
            free(tokens[i]->value);
        }
        free(tokens[i]);
    }
    free(tokens);
}

#endif // LEXER_H

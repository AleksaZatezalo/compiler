#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Global token list and current position
Token **tokens;
int current_token = 0;

// ===== PARSER FUNCTIONS =====

// Get current token without consuming it
Token* peek_token() {
    return tokens[current_token];
}

// Get current token and advance to next
Token* next_token() {
    return tokens[current_token++];
}

// Verify current token type and consume it
void expect_token(TokenType type) {
    Token *token = peek_token();
    if (token->type != type) {
        fprintf(stderr, "Parse error: expected token type %d, got %d\n", type, token->type);
        exit(1);
    }
    next_token();
}

// Create a new AST node
ASTNode* create_node(NodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

// Parse expression: currently just integer literals
ASTNode* parse_expression() {
    Token *token = next_token();
    if (token->type != TOKEN_NUMBER) {
        fprintf(stderr, "Parse error: expected number in expression\n");
        exit(1);
    }
    
    ASTNode *node = create_node(NODE_EXPRESSION_INT);
    node->int_expr.value = atoi(token->value);
    return node;
}

// Parse statement: currently just return statements
ASTNode* parse_statement() {
    expect_token(TOKEN_RETURN);
    
    ASTNode *node = create_node(NODE_STATEMENT_RETURN);
    node->return_stmt.expression = parse_expression();
    
    expect_token(TOKEN_SEMICOLON);
    
    return node;
}

// Parse function: int main(void) { statement }
ASTNode* parse_function() {
    expect_token(TOKEN_INT);
    expect_token(TOKEN_MAIN);
    expect_token(TOKEN_LPAREN);
    expect_token(TOKEN_VOID);
    expect_token(TOKEN_RPAREN);
    expect_token(TOKEN_LBRACE);
    
    ASTNode *node = create_node(NODE_FUNCTION);
    node->function.name = strdup("main");
    node->function.body = parse_statement();
    
    expect_token(TOKEN_RBRACE);
    
    return node;
}

// Parse program: currently just one function
ASTNode* parse_program(Token **token_array) {
    tokens = token_array;
    current_token = 0;
    
    ASTNode *node = create_node(NODE_PROGRAM);
    node->program.function = parse_function();
    
    expect_token(TOKEN_EOF);
    
    return node;
}

// ===== AST MEMORY MANAGEMENT =====

void free_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
            free_ast(node->program.function);
            break;
            
        case NODE_FUNCTION:
            if (node->function.name) {
                free(node->function.name);
            }
            free_ast(node->function.body);
            break;
            
        case NODE_STATEMENT_RETURN:
            free_ast(node->return_stmt.expression);
            break;
            
        case NODE_EXPRESSION_INT:
            // No dynamic memory to free
            break;
    }
    
    free(node);
}

// ===== AST PRINTING (for debugging) =====

void print_ast(ASTNode *node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) printf("  ");
    
    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            print_ast(node->program.function, indent + 1);
            break;
            
        case NODE_FUNCTION:
            printf("Function: %s\n", node->function.name);
            print_ast(node->function.body, indent + 1);
            break;
            
        case NODE_STATEMENT_RETURN:
            printf("Return Statement\n");
            print_ast(node->return_stmt.expression, indent + 1);
            break;
            
        case NODE_EXPRESSION_INT:
            printf("Int Literal: %d\n", node->int_expr.value);
            break;
    }
}

// ===== MAIN (for testing parser standalone) =====

#ifdef PARSER_STANDALONE
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
    
    // Tokenize the source
    int token_count = 0;
    Token **tokens = tokenize(source, &token_count);
    
    // Print tokens (for debugging)
    print_tokens(tokens, token_count);
    
    // Parse the tokens
    printf("\n=== AST ===\n");
    ASTNode *ast = parse_program(tokens);
    print_ast(ast, 0);
    
    // Cleanup
    free_ast(ast);
    free_tokens(tokens, token_count);
    free(source);
    
    return 0;
}
#endif

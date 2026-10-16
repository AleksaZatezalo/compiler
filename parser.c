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

// AST Node Types
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_STATEMENT_RETURN,
    NODE_EXPRESSION_INT
} NodeType;

// Forward declarations
typedef struct ASTNode ASTNode;

// AST Node structure
struct ASTNode {
    NodeType type;
    union {
        // Program node
        struct {
            ASTNode *function;
        } program;
        
        // Function node
        struct {
            char *name;
            ASTNode *body;
        } function;
        
        // Return statement node
        struct {
            ASTNode *expression;
        } return_stmt;
        
        // Integer expression node
        struct {
            int value;
        } int_expr;
    };
};

// Global token list and current position
Token **tokens;
int current_token = 0;

// ===== LEXER FUNCTIONS =====

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
ASTNode* parse_program() {
    ASTNode *node = create_node(NODE_PROGRAM);
    node->program.function = parse_function();
    
    expect_token(TOKEN_EOF);
    
    return node;
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

// ===== MAIN =====

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
    
    // Lex the source - collect all tokens
    char *input = source;
    Token *token;
    int token_count = 0;
    int token_capacity = 100;
    tokens = malloc(sizeof(Token*) * token_capacity);
    
    while (1) {
        token = lex(&input);
        tokens[token_count++] = token;
        
        if (token->type == TOKEN_EOF) {
            break;
        }
        
        if (token_count >= token_capacity) {
            token_capacity *= 2;
            tokens = realloc(tokens, sizeof(Token*) * token_capacity);
        }
    }
    
    printf("=== Tokens ===\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token: type=%d, value='%s'\n", tokens[i]->type, 
               tokens[i]->value ? tokens[i]->value : "NULL");
    }
    
    // Parse the tokens
    printf("\n=== AST ===\n");
    ASTNode *ast = parse_program();
    print_ast(ast, 0);
    
    // Cleanup
    free(source);
    
    return 0;
}

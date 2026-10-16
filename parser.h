#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// AST Node Types
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_STATEMENT_RETURN,
    NODE_EXPRESSION_INT
} NodeType;

// Forward declaration
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

// Parser function declarations
ASTNode* parse_program(Token **tokens);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

#endif // PARSER_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "codegen.h"

// Helper to append to a dynamically growing string
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} StringBuilder;

static StringBuilder* sb_create() {
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    sb->capacity = 1024;
    sb->length = 0;
    sb->data = malloc(sb->capacity);
    sb->data[0] = '\0';
    return sb;
}

static void sb_append(StringBuilder *sb, const char *str) {
    size_t str_len = strlen(str);
    while (sb->length + str_len + 1 > sb->capacity) {
        sb->capacity *= 2;
        sb->data = realloc(sb->data, sb->capacity);
    }
    strcpy(sb->data + sb->length, str);
    sb->length += str_len;
}

static void sb_append_fmt(StringBuilder *sb, const char *fmt, ...) {
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    sb_append(sb, buffer);
}

static char* sb_finalize(StringBuilder *sb) {
    char *result = sb->data;
    free(sb);
    return result;
}

// Forward declarations
static void generate_expression(ASTNode *node, StringBuilder *sb);
static void generate_statement(ASTNode *node, StringBuilder *sb);
static void generate_function(ASTNode *node, StringBuilder *sb);

// Generate code for an integer expression
static void generate_expression(ASTNode *node, StringBuilder *sb) {
    if (node->type == NODE_EXPRESSION_INT) {
        sb_append_fmt(sb, "    movl    $%d, %%eax\n", node->int_expr.value);
    }
}

// Generate code for a statement
static void generate_statement(ASTNode *node, StringBuilder *sb) {
    if (node->type == NODE_STATEMENT_RETURN) {
        generate_expression(node->return_stmt.expression, sb);
        sb_append(sb, "    popq    %rbp\n");
        sb_append(sb, "    ret\n");
    }
}

// Generate code for a function
static void generate_function(ASTNode *node, StringBuilder *sb) {
    if (node->type != NODE_FUNCTION) return;
    
    // Function label and prologue
    sb_append_fmt(sb, "    .globl %s\n", node->function.name);
    sb_append_fmt(sb, "%s:\n", node->function.name);
    sb_append(sb, "    pushq   %rbp\n");
    sb_append(sb, "    movq    %rsp, %rbp\n");
    
    // Generate body
    generate_statement(node->function.body, sb);
}

// Main code generation function
char* generate_code(ASTNode *ast) {
    if (!ast || ast->type != NODE_PROGRAM) {
        fprintf(stderr, "Error: Invalid AST for code generation\n");
        exit(1);
    }
    
    StringBuilder *sb = sb_create();
    
    // Add assembly directives
    sb_append(sb, "    .section .text\n");
    
    // Generate function
    generate_function(ast->program.function, sb);
    
    return sb_finalize(sb);
}

// Write assembly code to file
void write_assembly(const char *code, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: cannot open output file %s\n", filename);
        exit(1);
    }
    
    fprintf(file, "%s", code);
    fclose(file);
}

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

// Read source file into string
static char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(1);
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *source = malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    
    return source;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source.c> <output.s>\n", argv[0]);
        return 1;
    }
    
    const char *input_file = argv[1];
    const char *output_file = argv[2];
    
    // Read the source file
    char *source = read_file(input_file);
    
    // Lexical analysis
    int token_count = 0;
    Token **tokens = tokenize(source, &token_count);
    
    // Parsing
    ASTNode *ast = parse_program(tokens);
    
    // Code generation
    char *assembly = generate_code(ast);
    
    // Print assembly to stdout
    printf("=== Generated Assembly ===\n");
    printf("%s", assembly);
    printf("==========================\n\n");
    printf("Writing to file: %s\n", output_file);
    
    // Write to file
    write_assembly(assembly, output_file);
    
    printf("Success! Assembly written to %s\n", output_file);
    
    // Cleanup
    free(assembly);
    free_ast(ast);
    free_tokens(tokens, token_count);
    free(source);
    
    return 0;
}

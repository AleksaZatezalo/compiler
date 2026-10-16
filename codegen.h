#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

// Generate assembly code from AST and return as string
char* generate_code(ASTNode *ast);

// Write assembly code to file
void write_assembly(const char *code, const char *filename);

#endif // CODEGEN_H

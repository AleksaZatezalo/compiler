#ifndef ASSEMBLER_H
#define ASSEMBLER_H

// Assemble assembly code to executable
// Takes assembly string and output filename
// Returns 0 on success, -1 on error
int assemble(const char *assembly_code, const char *output_file);

#endif // ASSEMBLER_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "assembler.h"

// Assemble assembly code to executable using gcc (via fork/exec)
int assemble(const char *assembly_code, const char *output_file) {
    // Step 1: Write assembly to temporary file
    const char *temp_asm = "/tmp/compiler_temp.s";
    
    FILE *asm_file = fopen(temp_asm, "w");
    if (!asm_file) {
        fprintf(stderr, "Error: Cannot create temporary assembly file\n");
        return -1;
    }
    
    fprintf(asm_file, "%s", assembly_code);
    fclose(asm_file);
    
    // Step 2: Assemble and link with gcc
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Error: Fork failed\n");
        return -1;
    }
    
    if (pid == 0) {
        // Child process: run 'gcc'
        execlp("gcc", "gcc", temp_asm, "-o", output_file, NULL);
        // If execlp returns, it failed
        fprintf(stderr, "Error: Failed to execute 'gcc'\n");
        exit(1);
    } else {
        // Parent process: wait for gcc
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Error: Compilation failed\n");
            return -1;
        }
    }
    
    // Step 3: Clean up temporary file
    unlink(temp_asm);
    
    return 0;
}

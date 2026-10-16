#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "assembler.h"

// Assemble assembly code to executable using 'as' (assembler) and 'ld' (linker)
// This does NOT use GCC - it directly invokes the GNU binutils tools
int assemble(const char *assembly_code, const char *output_file) {
    // Temporary files
    const char *temp_asm = "/tmp/compiler_temp.s";
    const char *temp_obj = "/tmp/compiler_temp.o";
    
    // Step 1: Write assembly to temporary file
    FILE *asm_file = fopen(temp_asm, "w");
    if (!asm_file) {
        fprintf(stderr, "Error: Cannot create temporary assembly file\n");
        return -1;
    }
    
    fprintf(asm_file, "%s", assembly_code);
    fclose(asm_file);
    
    // Step 2: Assemble with 'as' (GNU assembler)
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Error: Fork failed\n");
        unlink(temp_asm);
        return -1;
    }
    
    if (pid == 0) {
        // Child process: run 'as'
        execlp("as", "as", temp_asm, "-o", temp_obj, NULL);
        // If execlp returns, it failed
        fprintf(stderr, "Error: Failed to execute 'as'\n");
        exit(1);
    } else {
        // Parent process: wait for assembler
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Error: Assembly failed\n");
            unlink(temp_asm);
            return -1;
        }
    }
    
    // Step 3: Link with 'ld' (GNU linker)
    pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Error: Fork failed\n");
        unlink(temp_asm);
        unlink(temp_obj);
        return -1;
    }
    
    if (pid == 0) {
        // Child process: run 'ld' (GNU linker)
        // 
        // Explanation of linker arguments:
        // -dynamic-linker: Specifies the runtime dynamic linker (ld.so)
        // crt1.o:  C runtime startup code (calls main, sets up argc/argv)
        // crti.o:  Initialization code (constructor prologue)
        // Our .o:  The object file we just assembled
        // crtn.o:  Finalization code (constructor epilogue)  
        // -lc:     Link against the C standard library (libc.so.6)
        // -o:      Output executable filename
        //
        execlp("ld", "ld",
               "-dynamic-linker", "/lib64/ld-linux-x86-64.so.2",  // Runtime linker
               "/usr/lib/x86_64-linux-gnu/crt1.o",                // C runtime start
               "/usr/lib/x86_64-linux-gnu/crti.o",                // Init prologue
               temp_obj,                                           // Our object file
               "/usr/lib/x86_64-linux-gnu/crtn.o",                // Init epilogue
               "-lc",                                              // C standard library
               "-o", output_file,                                 // Output executable
               NULL);
        // If execlp returns, it failed
        fprintf(stderr, "Error: Failed to execute 'ld'\n");
        exit(1);
    } else {
        // Parent process: wait for linker
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Error: Linking failed\n");
            unlink(temp_asm);
            unlink(temp_obj);
            return -1;
        }
    }
    
    // Step 4: Clean up temporary files
    unlink(temp_asm);
    unlink(temp_obj);
    
    return 0;
}

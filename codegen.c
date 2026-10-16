/*Converts AST to Assembly*/

/*
AST Node                        Assembly Construct
Program(function_definition)    Program(function_definition)
Function(name, body)            Function(name, instructions)
Retrun(exp)                     Mov(exp, Register)
                                Ret
Constant(int)                   Imm(int)
*/

/*
We are compiling on linux. We will have to add the following to the end of a file:
.section .note.GBU-stack,",@progbits"
*/
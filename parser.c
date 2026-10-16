/*
Will be built using ASTL - Zephyr Abstract Santax Description Language
*/

/*
Abstract Santax description of this parser
program = Program(function_definition)
function_definition = Function(identifier name, statement body)
statement = Return(exp)
exp = Constant(int)
*/

/*
Formal Grammer:
<program> ::= <function>
<function> ::= "int" <identifier> "("void")" "{"<statement>"}"
<statement> ::= "return" <exp> ";"
<exp> ::= <int>
<int> ::= ? A constant token ?
*/
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

/*Parsing Pseudocode*/
parse_statment(tokens):
    expect("return", tokens)
    return_val = parse_exp(tokens)
    expect(";", tokens)
    return Return(return_val)

expect(expected, tokens):
    actual = take_token(tokens)
    if actual != expected:
        fail("Santax Error")

/*Pretty printer*/
It should have a pretty printer function that prints the AST in this way:
Program(
    Function(
        name="main"
        body=Return(
            Constant(2)
        )
    )
)
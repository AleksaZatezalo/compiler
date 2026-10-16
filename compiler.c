/*
The following tokens are included:
int
main 
(
void
)
{
return
2
;
}
*/

/*Tokens will be caught through regular expresssions*/

while input isn't empty:
    if input starts with whitespace:
            trim whitespace from start of input
    else:
        find logest match at start of regex from regex table
        if no match is found raise an error
        convert matching substring into a token
        remove matching substring from start of the input
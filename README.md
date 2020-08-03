# lexical-analyzer-for-psi-lang
A lexical analyzer for custom defined PSI Language, developed for Programming Languages Course.


## Objective
Creating the Lexical Analyzer phase of PSI++ Compiler in C. Analyzer should get the ‘.psi’ file and output the produced tokens to a ‘.lex’ file.

## Detailed Description of Token Handling

### Keywords
PSI++ Language has 18 different keywords. For this program’s consideration it has to be surrounded by whitespace, and match the predefined set of keywords. They are tokenized as ‘Keyword’
### Identifiers
Identifiers in PSI++ are arbitrarily named so it doesn’t have any predefined set but rules. Any identifier surrounded by whitespace or operators, which has a length less than 30 characters, starting with alphabetic characters and containing only alphanumeric characters and underscore is tokenized as ‘Identifier’ in Analyzer.  
### String Constants
Any set of characters which are surrounded by double quote is tokenized as ‘StringConst’ in Analyzer.
### Integer Constants
Any sets of digit surrounded by whitespace which length is less than 10 is considered as ‘IntConst’ token.
### Operators
When the one of the operators in predefined set of operators found in source code of any PSI++ program, it is considered to be an operator and tokenized as ‘Operator’ by the Analyzer.
### End of Line
Semicolons are tokenized as ‘EndOfLine’.
### Brackets
Parenthesis, Curly Brackets and Square Brackets are tokenized by Analyzer with their respective names.
### Comments
This Analyzer ignores the comments which are denoted any text between ‘/*’ and ‘*/’. It doesn’t show up in the ‘.lex’ file.

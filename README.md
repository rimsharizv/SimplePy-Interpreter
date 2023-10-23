# SimplePy Interpreter
Author: Rimsha Rizvi

## Description
SimplePy is a minimalistic Python interpreter that supports integer and string data types, the print() function, and simple assignment statements.

## Compilation
g++ -o main main.cpp

## Usage
To run SimplePy, you need to provide the SimplePy program file and the size of the memory array as command-line arguments: ./main filename.py memorysize
1. filename.py: The SimplePy program file.
2. memorysize: The size of the memory array.

## Function Descriptions
1. searchRAM: Searches memory for a variable matching the given identifier and returns the index if found, -1 if not.
2. retrieveElement: Retrieves an expression element, which can be a variable or a literal (integer or string), and returns the value as a string along with the value's type.
3. doPrintFunction: Executes the print() function in Python, consuming and executing the function tokens.
4. performRelationalOp: Performs relational operations such as <, <=, >, >=, ==, != and returns "0" (false) or "1" (true) as a string.
5. performOp: Performs operations such as + or < and returns the result as a string. The result's type is returned via the resultType parameter, which is passed by reference.
6. evaluateExpr: Evaluates an expression such as x * y and returns the value and its type ("int" or "str"). The program counter (PC) is advanced as tokens are consumed and executed.
7. executeStmt: Executes the statement denoted by the program counter (PC) in the program array, consuming and executing the tokens.

## Copyright and Plagiarism Notice
All content in this repository, including code, documentation, and other materials, is the property of Rimsha Rizvi. This work is submitted for CS 251 - Data Structures at University of Illinois at Chicago.
Plagiarism is a serious offense and is strictly prohibited. You may not use, copy, or submit this work as your own. Any instances of plagiarism or unauthorized use will be reported to the relevant academic or professional authorities.
If you would like to use this work for educational or other non-commercial purposes, please contact the author for permission.

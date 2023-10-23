/*main.cpp*/

//
// SimplePy (simple python) interpreter. The language only supports int and str data
// types, the print() function, and simple assignment statements.  BNF is below.
//
// Author: Rimsha Rizvi
//
// Originally Written by: Prof. Hummel, U. of Illinois, Chicago, Spring 2021
//

#include <iostream>
#include <string>
#include <cmath>    // pow
#include <cctype>   // isdigit

#include "token.h"

using namespace std;

struct Variable
{
   string  Name;
   string  Type;
   string  Value;
};

struct Result
{
   string  Value;
   string  Type;
};


// //
// // Function declarations:
// //
// int searchRAM(Variable memory[], int N, string identifier);
// Result retrieveElement(Token program[], int PC, Variable memory[], int numVars);
// void doPrintFunction(Token program[], int& PC, Variable memory[], int numVars);
// string performRelationalOp(Token op, string leftValue, string leftType, string rightValue, string rightType);
// Result performOp(Token op, string leftValue, string leftType, string rightValue, string rightType);
// Result evaluateExpr(Token program[], int& PC, Variable memory[], int numVars);
// void executeStmt(Token program[], int& PC, Variable memory[], int& numVars);


//
// searchRAM
//
// Searches memory for the variable matching the given identifier.
// Returns the index if found, -1 if not.
//
int searchRAM(Variable memory[], int NumVars, string identifier)
{
   for (int i = 0; i < NumVars; i++)
   {
      if (memory[i].Name == identifier)
         return i;
   }
   return -1;
}


//
// retrieveElement
//
// Retrieves an expression element, which is a variable or a literal (int
// or string). Returns the value as a string, along with the value's type.
//
Result retrieveElement(Token program[], int PC, Variable memory[], int NumVars)
{
   Result result;;

   if (program[PC].ID == TokenID::INT_LITERAL)
   {
      result.Value = program[PC].Value;
      result.Type = "int";
   }
   else if (program[PC].ID == TokenID::STR_LITERAL)
   {
      result.Value = program[PC].Value;
      result.Type = "str";
   }
   else if (program[PC].ID == TokenID::IDENTIFIER)
   {
      string identifier = program[PC].Value;

      int index = searchRAM(memory, NumVars, identifier);

      result.Value = memory[index].Value;
      result.Type = memory[index].Type;
   }
   else
   {
      // 
      // sanity check: this shouldn't happen but let's make sure:
      //
      cout << "**Internal Error: was expecting an <element>, encountered token=" << program[PC] << endl;

      result.Value = "0";  // with errors the default is integer 0
      result.Type = "int";
   }

   return result;
}


//
// doPrintFunction
//
// Executes the print() function in Python. The PC is advanced as the 
// function tokens are consumed and executed.
//
void doPrintFunction(Token program[], int& PC, Variable memory[], int NumVars)
{
   //
   // print(param, param, ...) --- 0 or more  params
   //
   // <funcall> ::= <functname> "(" <params> ")"
   // <params>  ::= (<param> ("," < param > )*) ?
   // <param>   ::= <element>
   // <element> ::= <varname>
   //             | <literal>
   //
   PC++;  // consume print token
   PC++;  // consume opening (

   // parameter or )?
   while (program[PC].ID != TokenID::RIGHT_PAREN)
   {
      //
      // we have a parameter, we need to output it:
      //
      // <varname> ::= <identifier>
      // <literal> ::= <integer>
      //             | <string>
      //

      //
      // a parameter is an <element>, so retrive it:
      //
      Result element = retrieveElement(program, PC, memory, NumVars);
      
      cout << element.Value;

      PC++;  // consume token

      //
      // do we have another parameter?
      //
      if (program[PC].ID == TokenID::COMMA)  // we have another parameter:
      {
         cout << " ";  // python outputs " " between values

         PC++;  // consume comma so parameter or ) is next
      }
   }//while

   PC++;  // consume closing )

   //
   // finish with end-of-line
   //
   cout << endl;
}


//
// performRelationalOp
//
// Performs the relational operation denoted by op, i.e. one of <, <=, >, >=, ==, and !=.
// The result is returned as a string, either "0" (false) or "1" (true). You know the 
// return type, it's always integer.
//
string performRelationalOp(Token op, string leftValue, string leftType, string rightValue, string rightType)
{
   int result = 0;  // in case the op doesn't match, error case:

   if (leftType == "int" && rightType == "int")
   {
      //
      // int and int
      //
      int lhs = stoi(leftValue);
      int rhs = stoi(rightValue);

      switch (op.ID)
      {
      case TokenID::LT:
         result = (lhs < rhs) ? 1 : 0;
         break;
      case TokenID::LTE:
         result = (lhs <= rhs) ? 1 : 0;
         break;
      case TokenID::GT:
         result = (lhs > rhs) ? 1 : 0;
         break;
      case TokenID::GTE:
         result = (lhs >= rhs) ? 1 : 0;
         break;
      case TokenID::EQUAL_EQUAL:
         result = (lhs == rhs) ? 1 : 0;
         break;
      case TokenID::NOT_EQUAL:
         result = (lhs != rhs) ? 1 : 0;
         break;
      default:
         cout << "**Internal Error: unknown rel operator, encountered token=" << op << endl;
      }
   }
   else
   {
      //
      // str and str
      //
      switch (op.ID)
      {
      case TokenID::LT:
         result = (leftValue < rightValue) ? 1 : 0;
         break;
      case TokenID::LTE:
         result = (leftValue <= rightValue) ? 1 : 0;
         break;
      case TokenID::GT:
         result = (leftValue > rightValue) ? 1 : 0;
         break;
      case TokenID::GTE:
         result = (leftValue >= rightValue) ? 1 : 0;
         break;
      case TokenID::EQUAL_EQUAL:
         result = (leftValue == rightValue) ? 1 : 0;
         break;
      case TokenID::NOT_EQUAL:
         result = (leftValue != rightValue) ? 1 : 0;
         break;
      default:
         cout << "**Internal Error: unknown rel operator, encountered token=" << op << endl;
      }
   }

   return to_string(result);
}


//
// performOp
//
// Performs the operation denoted by op, e.g. "+" or "<". Returns the result as
// a string, and returns the result's type via the resultType parameter which is 
// passed-by-reference.
//
Result performOp(Token op, string leftValue, string leftType, string rightValue, string rightType)
{
   Result result;

   //
   // perform operation:
   //
   if (op.ID == TokenID::PLUS)
   {
      //
      // this can be int or str:
      //
      if (leftType == "int" && rightType == "int")
      {
         int lhs = stoi(leftValue);
         int rhs = stoi(rightValue);
         result.Value = to_string(lhs + rhs);
         result.Type = "int";
      }
      else  // str and str
      {
         result.Value = leftValue + rightValue;  // string concatenation
         result.Type = "str";
      }
   }
   else if (op.ID == TokenID::MINUS)
   {
      // int and int
      int lhs = stoi(leftValue);
      int rhs = stoi(rightValue);
      result.Value = to_string(lhs - rhs);
      result.Type = "int";
   }
   else if (op.ID == TokenID::MULT)
   {
      // int and int
      int lhs = stoi(leftValue);
      int rhs = stoi(rightValue);
      result.Value = to_string(lhs * rhs);
      result.Type = "int";
   }
   else if (op.ID == TokenID::DIV)
   {
      // int and int
      //
      // NOTE: python checks for divide by 0 and throws an error,
      // we're going to just silently evaluate to 0.
      //
      int lhs = stoi(leftValue);
      int rhs = stoi(rightValue);
      if (rhs == 0)
         result.Value = "0";
      else
         result.Value = to_string(lhs / rhs);

      result.Type = "int";
   }
   else if (op.ID == TokenID::MOD)
   {
      // int and int
      //
      // NOTE: python checks for divide by 0 and throws an error,
      // we're going to just silently evaluate to 0.
      //
      int lhs = stoi(leftValue);
      int rhs = stoi(rightValue);
      if (rhs == 0)
         result.Value = "0";
      else
         result.Value = to_string(lhs % rhs);

      result.Type = "int";
   }
   else if (op.ID == TokenID::POWER)
   {
      // int and int
      int lhs = stoi(leftValue);
      int rhs = stoi(rightValue);
      int value = static_cast<int>(pow(lhs, rhs));
      result.Value = to_string(value);
      result.Type = "int";
   }
   else
   {
      //
      // then it should be a relational operator (and the function
      // will check and report an internal error if not):
      //
      result.Value = performRelationalOp(op, leftValue, leftType, rightValue, rightType);
      result.Type = "int";
   }

   return result;
}


//
// evaluateExpr
//
// Executes an expr such as "x * y". The value is returned, 
// along with the type of this value --- "int" or "str". The
// PC is advanced as the tokens are consumed and executed.
//
Result evaluateExpr(Token program[], int& PC, Variable memory[], int NumVars)
{
   Result  result;
   //
   // <expr>    ::= <element> (<op> <element>)?
   //             | <funcall>
   // <element> ::= <varname>
   //             | <literal>
   //
   Result left;

   //
   // let's get the first element of the expression --- think of 
   // this as the "left" value of an expression like x + y:
   //
   left = retrieveElement(program, PC, memory, NumVars);

   PC++;  // consume element token

   //
   // is there an operation following?
   //
   if (program[PC].ID == TokenID::PLUS ||
      program[PC].ID == TokenID::MINUS ||
      program[PC].ID == TokenID::MULT ||
      program[PC].ID == TokenID::DIV ||
      program[PC].ID == TokenID::MOD ||
      program[PC].ID == TokenID::POWER ||
      program[PC].ID == TokenID::LT ||
      program[PC].ID == TokenID::LTE ||
      program[PC].ID == TokenID::GT ||
      program[PC].ID == TokenID::GTE ||
      program[PC].ID == TokenID::EQUAL_EQUAL ||
      program[PC].ID == TokenID::NOT_EQUAL)
   {
      Token op = program[PC];  // save operator

      PC++;  // consume operator

      //
      // now we need RHS of expression:
      //
      Result right;

      right = retrieveElement(program, PC, memory, NumVars);

      PC++;  // consume element token

      //
      // now perform the operation:
      //
      result = performOp(op, left.Value, left.Type, right.Value, right.Type);
   }
   else
   {
      //
      // no operation, we just have the left as the result:
      //
      result = left;
   }
   
   return result;
}


//
// executeStmt
//
// Executes the statement denoted by PC in the program array. The 
// PC is advanced as the tokens are consumed and executed.
//
void executeStmt(Token program[], int& PC, Variable memory[], int& NumVars)
{
   //
   // <stmt> ::= <funcall>
   //          | <assign>
   //
   // <funcall> ::= <functname> "(" <params> ")"
   // <assign>  ::= <varname> "=" <expr>
   //
   // both start with an identifer:
   //
   if (program[PC].ID == TokenID::IDENTIFIER)
   {
      string identifier = program[PC].Value;

      if (identifier == "print")
      {
         doPrintFunction(program, PC, memory, NumVars);
      }
      else
      {
         //
         // We have an assignment statement:
         //
         // <assign> ::= <varname> "=" <expr>
         //
         string identifier = program[PC].Value;

         PC++;  // consume identifier token
         PC++;  // consume =

         //
         // evaluate the expression
         //
         Result result;

         result = evaluateExpr(program, PC, memory, NumVars);

         // 
         // now do the assignment...  does the variable exist?
         // if not, then add to memory, else update existing
         // variable's value and type:
         //
         int index = searchRAM(memory, NumVars, identifier);

         if (index < 0)
         {
            index = NumVars;  // add to end of memory
            NumVars++;        // another variable!
            
            memory[index].Name = identifier;
         }

         memory[index].Value = result.Value;
         memory[index].Type = result.Type;
      }
   }//identifier
   else
   {
      // 
      // sanity check: this shouldn't happen but let's make sure:
      //
      cout << "**Internal Error: unknown start of stmt, token=" << program[PC] << endl;

      PC++;  // consume the token, whatever it is
   }
}


// ##################################################################################
//
// main filename memorysize
//
int main(int argc, char* argv[])
{
   // 
   // 1. process command-line args:
   //
   // argv[0]: ./main
   // argv[1]: filename.py
   // argv[2]: memorysize
   //
   if (argc != 3)
   {
      cout << "usage: ./main filename.py memorysize" << endl;
      return 0;
   }

   string filename(argv[1]);  // this is your SimplePy program file
   int    MemorySize = stoi(argv[2]);  // this is size for Memory array

   //
   // 2. input python program and store as an array of tokens:
   //
   int ProgramSize;

   Token* program = inputSimplePy(filename, ProgramSize);

   if (program == nullptr)
   {
      cout << "**Error, unable to open program file '" << filename << "'" << endl;
      return 0;
   }

   //
   // 3. create a memory for python variables:
   //
   Variable* memory = new Variable[MemorySize];

   //
   // 4. now start executing the program, the model is do what the
   // computer HW does, i.e. statement by statement:
   //
   int PC = 0;  
   int NumVars = 0;

   while (program[PC].ID != TokenID::EOS)
   {
      executeStmt(program, PC, memory, NumVars);
   }

   //
   // done:
   //
   delete[] program;
   delete[] memory;

   return 0;
}

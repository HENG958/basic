/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "parser.hpp"
#include <cstddef>
#include <cstdio>


/* Implementation of the Statement class */
bool IsVarLegal (std::string str);
int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

void Statement::execute(EvalState &state, Program &program, int &lineNumber) 
{
  return;
}

void Statement::parsetransfer(TokenScanner &scanner) 
{
  return;
}

void RemStmt::execute(EvalState &state, Program &program, int &lineNumber) 
{
  lineNumber = program.getNextLineNumber(lineNumber);
  return;
}

void RemStmt::parsetransfer(TokenScanner &scanner) {
  return;
}

void LetStmt::execute(EvalState &state, Program &program, int &lineNumber)
{
  if (!IsVarLegal(var)) error("SYNTAX ERROR");
  state.setValue(var, exp->eval(state));
  lineNumber = program.getNextLineNumber(lineNumber);
  return;
}

void LetStmt::parsetransfer(TokenScanner &scanner)
{
  var = scanner.nextToken();
  if (scanner.nextToken() != "=") error("SYNTAX ERROR");
  exp = parseExp(scanner);
}

LetStmt::~LetStmt()
{
  delete exp;
  return;
}

void PrintStmt::execute(EvalState &state, Program &program, int &lineNumber)
{
  std::cout << exp->eval(state) << std::endl;
  lineNumber = program.getNextLineNumber(lineNumber);
  return;
}

void PrintStmt::parsetransfer(TokenScanner &scanner) 
{
  exp = parseExp(scanner);
  return;
}

PrintStmt::~PrintStmt() 
{
  delete exp;
  return;
}

void InputStmt::execute(EvalState &state, Program &program, int &lineNumber)
{
  std::cout << " ? ";
  std::string str;
  getline(std::cin, str);
  state.setValue(var, stringToInteger(str));
  lineNumber = program.getNextLineNumber(lineNumber);
  return;
}

void InputStmt::parsetransfer(TokenScanner &scanner) 
{
  var = scanner.nextToken();
  return;
}

void EndStmt::execute(EvalState &state, Program &program, int &lineNumber) 
{
  lineNumber = -1;
  return;
}

void EndStmt::parsetransfer(TokenScanner &scanner) 
{
  return;
}

void GotoStmt::execute(EvalState &state, Program &program, int &lineNumber)
{
  if (program.getParsedStatement(this->lineNumber) == nullptr) error("LINE NUMBER ERROR");
  lineNumber = this->lineNumber;
  return;
}

void GotoStmt::parsetransfer(TokenScanner &scanner)
{
  lineNumber = stringToInteger(scanner.nextToken());
  return;
}

void IfStmt::execute(EvalState &state, Program &program, int &lineNumber)
{
  if (program.getParsedStatement(this->lineNumber) == nullptr) error("LINE NUMBER ERROR");
  if (op == '=')
  {
    if (left->eval(state) == right->eval(state)) 
      lineNumber = this->lineNumber; 
    else 
      lineNumber = program.getNextLineNumber(lineNumber);
  }
  else if (op == '<')
  {
    if (left->eval(state) < right->eval(state)) 
      lineNumber = this->lineNumber;
    else 
      lineNumber = program.getNextLineNumber(lineNumber);
  }
  else if (op == '>')
  {
    if (left->eval(state) > right->eval(state)) 
      lineNumber = this->lineNumber;
    else 
      lineNumber = program.getNextLineNumber(lineNumber);
  }
  else error("SYNTAX ERROR");

  if (program.getParsedStatement(lineNumber) == nullptr) error("LINE NUMBER ERROR");
  return;
}

void IfStmt::parsetransfer(TokenScanner &scanner)
{
  std::string l, r;
  TokenScanner temp_scanner;
  while (l.back() != '=' && l.back() != '<' && l.back() != '>') l += scanner.nextToken();
  op = l.back();
  l.pop_back();
  temp_scanner.setInput(l);
  left = parseExp((temp_scanner));
  std::string temp_str;
  while(1)
  {
    if (!scanner.hasMoreTokens()) 
      error("SYNTAX ERROR");

    temp_str = scanner.nextToken();
    if (temp_str == "THEN") 
        break;
    
    r += temp_str;
  }
  temp_scanner.setInput(r);
  right = parseExp(temp_scanner);
  if (temp_str != "THEN") 
    error("SYNTAX ERROR");
  else  
    lineNumber = stringToInteger(scanner.nextToken());
  return;
}

IfStmt::~IfStmt()
{
  delete left;
  delete right;
  return;
}

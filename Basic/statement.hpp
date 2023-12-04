/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */
#pragma once
#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include"parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program, int &lineNumber) = 0;
    virtual void parsetransfer(TokenScanner &scanner) = 0;
};

class RemStmt : public Statement
{
public:
    RemStmt() = default;
    virtual ~RemStmt() = default;
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

class LetStmt : public Statement
{
public:
    std::string var;
    Expression *exp;
    //int lineNumber;
    LetStmt() = default;
    virtual ~LetStmt();
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

class PrintStmt : public Statement
{
public:
    Expression *exp;
    PrintStmt() = default;
    virtual ~PrintStmt();
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

class InputStmt : public Statement
{
public: 
    std::string var;
    InputStmt() = default;
    virtual ~InputStmt() = default;
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

class GotoStmt : public Statement 
{
public:
    int lineNumber;
    GotoStmt() = default;
    virtual ~GotoStmt() = default;
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

class IfStmt : public Statement
{
public:
    int lineNumber;
    Expression *left, * right;
    char op;
    IfStmt() = default;
    virtual ~IfStmt();
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

class EndStmt : public Statement
{
public:
    int lineNumber;
    EndStmt() = default;
    virtual ~EndStmt() = default;
    virtual void execute(EvalState &state, Program &program, int &lineNumber);
    virtual void parsetransfer(TokenScanner &scanner);
};

const std::vector<std::string> KEY_WORDS = {"REM",  "LET", "PRINT", "INPUT", "END",  "GOTO", "IF","THEN", "RUN", "LIST",  "CLEAR", "QUIT", "HELP"};
/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

#endif

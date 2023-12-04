/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */


bool IsVarLegal (std::string str)
{
  for (int i = 0; i < KEY_WORDS.size(); i++)
  {
    if (str == KEY_WORDS[i]) return false;
  }
  return true;
}

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string tok = scanner.nextToken();
    if (tok == "LET")
    {
        std::string var = scanner.nextToken();
        if (!IsVarLegal(var)) error("SYNTAX ERROR");
        if (scanner.nextToken() != "=") error("SYNTAX ERROR");
        Expression *exp = parseExp(scanner);
        int temp_value;
        try 
        {
            temp_value = exp->eval(state);
        } catch (ErrorException &ex)
        {
            delete exp;
            throw ex;
        }
        state.setValue(var, temp_value);
        delete exp;
    }
    else if (tok == "PRINT")
    {
        Expression *exp = parseExp(scanner);
        int temp_value;
        try 
        {
            temp_value = exp->eval(state);
        } catch (ErrorException &ex)
        {
            delete exp;
            throw ex;
        }
        std::cout << temp_value << std::endl;
        delete exp;
    }
    else if (tok == "INPUT")
    {
        std::string var = scanner.nextToken();
        std::cout << " ? ";
        std::string str;
        getline(std::cin, str);
        state.setValue(var, stringToInteger(str));
    }
    else if (tok == "RUN")
    {
        int linenum = program.getFirstLineNumber();
        while (linenum != -1)
        {
            Statement *stmt = program.getParsedStatement(linenum);
            stmt->execute(state, program, linenum);
        }
    }
    else if (tok == "LIST")
    {
        int linenum = program.getFirstLineNumber();
        while (linenum != -1)
        {
            std::cout << program.getSourceLine(linenum) << std::endl;
            linenum = program.getNextLineNumber(linenum);
        }
    }
    else if (tok == "CLEAR")
    {
        program.clear();
        state.Clear();
    }
    else if (tok == "QUIT")
    {
        exit(0);
    }
    else if (tok == "HELP")
    {
        std::cout << "我真的急死了" << std::endl;
    }
    else  
    {
        if (tok.empty()) return;

        if (scanner.getTokenType(tok) == NUMBER) 
        {
            int linenum = stringToInteger(tok);
            if (scanner.hasMoreTokens()) 
            {
                program.addSourceLine(linenum, line);
                Statement *stmt;
                std::string token = scanner.nextToken();
                if (token == "REM") 
                    stmt = new RemStmt;
                else if (token == "LET") 
                    stmt = new LetStmt; 
                else if (token == "PRINT") 
                    stmt = new PrintStmt;
                else if (token == "INPUT") 
                    stmt = new InputStmt;
                else if (token == "GOTO") 
                    stmt = new GotoStmt;
                else if (token == "IF") 
                    stmt = new IfStmt;
                else if (token == "END") 
                    stmt = new EndStmt;
                else 
                    error("SYNTAX ERROR");
                stmt->parsetransfer(scanner);
                program.setParsedStatement(linenum, stmt);
            } 
            else 
                program.removeSourceLine(linenum);
        } 
        else
            error("SYNTAX ERROR");
    }
}




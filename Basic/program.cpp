/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    while (!line_num.empty()) removeSourceLine(*line_num.begin());
    return;
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
  if (line_num.find(lineNumber) != line_num.end())
    removeSourceLine(lineNumber);
  //if (line_num.empty()) return;
  line_num.insert(lineNumber);
  line_text[lineNumber] = line;
  return;
}

void Program::removeSourceLine(int lineNumber) {
  if (line_num.find(lineNumber) != line_num.end())
  {
    line_num.erase(lineNumber);
    line_text.erase(lineNumber);
    delete line_syntax[lineNumber];
    line_syntax.erase(lineNumber);
  }
  else return;
}

std::string Program::getSourceLine(int lineNumber) {
  if (line_num.find(lineNumber) != line_num.end())
  {
    return line_text[lineNumber];
  }
  else return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
  auto it = line_num.find(lineNumber);
  if (it == line_num.end())
  {
    error ("SYNTAX ERROR5");
  }
  line_syntax[lineNumber] = stmt;
  return;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
  if (line_num.find(lineNumber) != line_num.end()) return line_syntax[lineNumber];
  else return nullptr;
}

int Program::getFirstLineNumber() {
  if (line_num.empty()) return -1;
  else return *line_num.begin();
}

int Program::getNextLineNumber(int lineNumber) {
  if (line_num.upper_bound(lineNumber) == line_num.end()) return -1;
  else return *line_num.upper_bound((lineNumber));
}

//more func to add
//todo



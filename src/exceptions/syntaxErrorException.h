
#ifndef SYNTAXERROREXCEPTION_H
#define SYNTAXERROREXCEPTION_H

#include <iostream>
#include <sstream>
#include "exception.h"

using namespace std;

struct SyntaxErrorException : Exception
{
    SyntaxErrorException(int line, int col);
};

SyntaxErrorException::SyntaxErrorException(int line, int col) : Exception()
{
    ostringstream ss;
    ss << "Syntax error on line " << line << " col " << col;

    this->exception = "SyntaxError";
    this->description = ss.str();
}

#endif
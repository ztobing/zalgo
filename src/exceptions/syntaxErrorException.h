
#ifndef SYNTAXERROREXCEPTION_H
#define SYNTAXERROREXCEPTION_H

#include <iostream>
#include "exception.h"

using namespace std;

struct SyntaxErrorException : Exception
{
    SyntaxErrorException(int line, int col);
};

SyntaxErrorException::SyntaxErrorException(int line, int col) : Exception()
{
    this->exception = "SyntaxError";
    this->description = "Syntax error on line x col y.";
}

#endif
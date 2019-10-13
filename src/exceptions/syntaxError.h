#ifndef SYNTAXERROR_H
#define SYNTAXERROR_H

#include <iostream>

#include "exception.h"

using namespace std;

struct SyntaxError : Exception
{
    SyntaxError(int, int, string);
};

SyntaxError::SyntaxError(int line, int col, string message) : Exception(line, col, "SyntaxError", message) {}

#endif
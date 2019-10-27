#ifndef UNDEFINEDREFERENCE_H
#define UNDEFINEDREFERENCE_H

#include <iostream>

#include "exception.h"

using namespace std;

struct UndefinedReference : Exception
{
    UndefinedReference(int, int, string, string);
};

UndefinedReference::UndefinedReference(int line, int col, string currentLine, string message) : Exception(line, col, currentLine, "UndefinedReference", message) {}

#endif
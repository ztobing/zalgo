#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>

using namespace std;

struct Exception
{
    int line, col;
    string currentLine, type, message;
    Exception(int, int, string, string, string);
};

Exception::Exception(int line, int col, string currentLine, string type, string message)
{
    this->line = line;
    this->col = col;
    this->type = type;
    this->currentLine = currentLine;
    this->message = message;
}

#endif
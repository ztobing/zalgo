#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <vector>

#include "lexer.h"
#include "value.h"
#include "ast.h"

using namespace std;

struct Function : Value
{
    Value parameters;
    AST operations;
    Function();
    Function(Value, AST);
};

Function::Function() : Value(T_NONE, "") {}

Function::Function(Value parameters, AST operations) : Value(T_FUNC, "")
{
    this->parameters = parameters;
    this->operations = operations;
}

#endif
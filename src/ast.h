#ifndef AST_H
#define AST_H

#define T_NONE 0

#include <iostream>

#include "token.h"

using namespace std;

struct AST
{
    AST *left;
    AST *right;
    Token value;
    AST();
    AST(Token);
    ~AST();
    void setLeft(Token);
    void setRight(Token);
};

AST::AST()
{
    left = right = NULL;
    value = Token(-1, -1, -1, "");
}

AST::AST(Token t)
{
    left = right = NULL;
    value = t;
}

AST::~AST()
{
    if (left != NULL) delete left;
    if (right != NULL) delete right;
}

void AST::setLeft(Token t)
{
    if (left != NULL) delete left;
    left = new AST(t);
}

void AST::setRight(Token t)
{
    if (right != NULL) delete right;
    right = new AST(t);
}

#endif
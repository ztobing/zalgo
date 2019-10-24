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
    AST(const AST&);
    AST(Token);
    ~AST();
    void setLeft(AST);
    void setRight(AST);
};

AST::AST()
{
    left = right = NULL;
    value = Token(-1, -1, -1, "");
}

AST::AST(const AST& a)
{
    left = a.left;
    right = a.right;
    value = a.value;
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

void AST::setLeft(AST a)
{
    if (left != NULL) delete left;
    left = new AST(a);
}

void AST::setRight(AST a)
{
    if (right != NULL) delete right;
    right = new AST(a);
}

#endif
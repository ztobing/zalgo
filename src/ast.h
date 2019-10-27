#ifndef AST_H
#define AST_H

#include <iostream>

#include "token.h"

using namespace std;

struct AST
{
    AST* left;
    AST* right;
    int type;
    string value;
    AST(int, string);
    AST(const AST&);
};

AST::AST(int type, string value)
{
    this->type = type;
    this->value = value;
    this->left = this->right = NULL;
}

AST::AST(const AST& a)
{
    this->type = a.type;
    this->value = a.value;
    this->left = a.left != NULL ? a.left : NULL;
    this->right = a.right != NULL ? a.right : NULL;
}

#endif
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
    ~AST();
};

AST::AST(int type, string value)
{
    this->type = type;
    this->value = value;
    this->left = this->right = nullptr;
}

AST::AST(const AST& a)
{
    this->type = a.type;
    this->value = a.value;
    this->left = a.left != nullptr ? new AST(*a.left) : nullptr;
    this->right = a.right != nullptr ? new AST(*a.right) : nullptr;
}

AST::~AST()
{
    // cout << "Deconstructor called for node " << value << endl;
    // cout << "Left: " << left << endl;
    // cout << "Right: " << right << endl;
    // if (left != nullptr) delete left;
    // if (right != nullptr) delete right;
}

#endif
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

struct Token
{
    int type;
    string value;
    Token(int type, string value);
};

Token::Token(int type, string value)
{
    this->type = type;
    this->value = value;
}

#endif
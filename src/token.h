#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

struct Token
{
    string tokenType, value;
    
    Token(string tokenType);
    Token(string tokenType, string value);
};

Token::Token(string tokenType)
{
    this->tokenType = tokenType;
    this->value = "";
}

Token::Token(string tokenType, string value) : Token(tokenType)
{
    this->value = value;
}

#endif
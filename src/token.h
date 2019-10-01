#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

struct Token
{
    string tokenType, value;
    
    Token(string tokenType);
    Token(string tokenType, string value);
    bool operator==(const Token &rhs) const;
    bool operator!=(const Token &rhs) const;
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

bool Token::operator==(const Token &rhs) const
{
    return (this->tokenType == rhs.tokenType) && (this->value == rhs.value);
}

bool Token::operator!=(const Token &rhs) const
{
    return (this->tokenType != rhs.tokenType) && (this->value != rhs.value);
}

#endif
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

struct Token
{
    int type;
    string value;
    int line, col;
    Token();
    Token(int line, int col, int type, string value);
};

Token::Token()
{
    type = 0;
    line = col = -1;
    value = "";
}

Token::Token(int line, int col, int type, string value)
{
    this->line = line;
    this->col = col;
    this->type = type;
    this->value = value;
}

#endif
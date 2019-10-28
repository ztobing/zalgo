#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

struct Token
{
    int type;
    string value, lineContent;
    int line, col;
    Token();
    Token(int line, int col, string, int type, string value);
};

Token::Token()
{
    type = 0;
    line = col = -1;
    value = lineContent = "";
}

Token::Token(int line, int col, string lineContent, int type, string value)
{
    this->line = line;
    this->col = col;
    this->lineContent = lineContent;
    this->type = type;
    this->value = value;
}

#endif
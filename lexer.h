#ifndef LEXER_H
#define LEXER_H

// Define tokens
#define T_NULL "NULL"
#define T_STR "STR"
#define T_INT "INT"
#define T_ADD "ADD"
#define T_SUB "SUB"
#define T_MUL "MUL"
#define T_DIV "DIV"
#define T_LPAREN "LPAREN"
#define T_RPAREN "RPAREN"
#define T_VAR "VAR"
#define T_FUNC "FUNC"
#define T_EOF "EOF"

#include <iostream>
#include <queue>
#include <fstream>
#include "token.h"

using namespace std;

class Lexer
{
    private:
        queue<Token> tokens;
        bool isAlpha(char c);
        bool isNumber(char c);
    public:
        Lexer();
        Lexer(string filePath);
        Token next();
};

Lexer::Lexer()
{
    // Add shell handling code
}

Lexer::Lexer(string filePath)
{
    ifstream fStream(filePath);

    // File validation
    if (!fStream.good())
    {
        cout << "Can't read file: " << filePath << endl;
        exit(1);
    }

    // Tokenize input
    string input;
    string token;
    string expectedTokenType = T_NULL;

    // Shit needed for string tokenizer
    bool isTagOpened = false;

    while (!fStream.eof())
    {
        fStream >> input;
        
        // Add space after every iteration if token is string
        if (isTagOpened)
        {
            token += " ";
        }

        // Check every char on string and tokenize it
        for (int i = 0; i < input.length(); i++)
        {

            if (isTagOpened)
            {
                if (input[i] == '"')
                {
                    tokens.push(Token(T_STR, token));
                    isTagOpened = false;
                    expectedTokenType = T_NULL;
                    token = "";
                }
                else
                {
                    token += input[i];
                    continue;
                }
            }
            else
            {
                switch(input[i])
                {
                    case '"':
                        expectedTokenType = T_STR;
                        isTagOpened = true;
                        break;
                    case '(':
                        tokens.push(Token(T_LPAREN));
                        break;
                    case ')':
                        tokens.push(Token(T_RPAREN));
                        break;
                    case '*':
                        tokens.push(Token(T_MUL));
                        break;
                    case '/':
                        tokens.push(Token(T_ADD));
                        break;
                    case '+':
                        tokens.push(Token(T_ADD));
                        break;
                    case '-':
                        tokens.push(Token(T_SUB));
                        break;
                }
            }
        }
    }

    cout << tokens.size() << endl;

    while (!tokens.empty())
    {
        Token token = tokens.front();
        cout << "Token(TYPE=" + token.tokenType + ", VALUE=" + token.value + ")" << endl;
        tokens.pop();
    }
}

Token Lexer::next()
{
    Token token = tokens.front();
    tokens.pop();
    return token;
}

bool Lexer::isAlpha(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool Lexer::isNumber(char c)
{
    return c >= 48 && c <= 57;
}

#endif
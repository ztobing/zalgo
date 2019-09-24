#ifndef LEXER_H
#define LEXER_H

// Define tokens
#define T_NULL "T_NULL"
#define T_STR "T_STR"
#define T_INT "T_INT"
#define T_ADD "T_ADD"
#define T_SUB "T_SUB"
#define T_MUL "T_MUL"
#define T_DIV "T_DIV"
#define T_LPAREN "T_LPAREN"
#define T_RPAREN "T_RPAREN"
#define T_EOF "T_EOF"

#include <iostream>
#include <queue>
#include <fstream>
#include "token.h"

using namespace std;

class Lexer
{
    private:
        queue<Token> tokens;
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
        
        // mindfuck
        if (expectedTokenType == T_STR)
        {
            token += " ";
        }

        // Check every char on string and tokenize it
        for (int i = 0; i < input.length(); i++)
        {
            cout << "Checking " << input[i] << endl;

            if (isTagOpened && input[i] == '"')
            {
                cout << "STRING CLOSE" << endl;
                tokens.push(Token(T_STR, token));
                isTagOpened = false;
                expectedTokenType = T_NULL;
                token = "";
            }
            else if (isTagOpened && expectedTokenType == T_STR)
            {
                token += input[i];
                continue;
            }

            switch(input[i])
            {
                case '"':
                    if (!isTagOpened)
                    {
                        cout << "STRING OPEN" << endl;
                        expectedTokenType = T_STR;
                        isTagOpened = true;
                    }
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

        // Reset variables if expected token is not string
        if (expectedTokenType != T_STR)
        {
            token = "";
            expectedTokenType = T_NULL;
        }
    }

    for (int i = 0; i < tokens.size() + 1; i++)
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

#endif
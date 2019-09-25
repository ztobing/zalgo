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

#define T_IF "IF"
#define T_ELSE "ELSE"
#define T_WHILE "WHILE"
#define T_FOR "FOR"
#define T_EXPR "EXPR"

#define T_EQUALS "EQUALS"
#define T_EEQUALS "EEQUALS"
#define T_GREATER "GREATER"
#define T_LESS "LESS"
#define T_GREATEREQUALS "GREATEREQUALS"
#define T_LESSEQUALS "LESSEQUALS"

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

    // String tokenizer variables
    bool isTagOpened = false;

    while (!fStream.eof())
    {
        fStream >> input;
        if (input == "if")
        {
            tokens.push(Token(T_IF));
            continue;
        }
        else if (input == "else")
        {
            tokens.push(Token(T_ELSE));
            continue;
        }
        else if (input == "for")
        {
            tokens.push(Token(T_FOR));
            continue;
        }
        else if (input == "while")
        {
            tokens.push(Token(T_WHILE));
            continue;
        }

        if (isAlpha(input[0]))
        {
            tokens.push(Token(T_VAR, input));
            continue;
        }
        
        // for (int i = 0; i < input.length(); i++)
        // {

        // }
    }

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

    //     // Add space after every iteration if token is string
    //     if (isTagOpened)
    //     {
    //         token += " ";
    //     }

    //     // Check every char on string and tokenize it
    //     for (int i = 0; i < input.length(); i++)
    //     {
    //         // Parse expression
    //         if (expectedTokenType == T_EXPR)
    //         {
                
    //         }

    //         // Parse if
    //         if (expectedTokenType == T_NULL and input[i] == 'i' and input[i + 1] == 'f' and input[i + 2] == ' ')
    //         {
    //             tokens.push(Token(T_IF));
    //             expectedTokenType = T_EXPR;
    //             i += 2;
    //             continue;
    //         }

    //         // Parse string
    //         if (isTagOpened)
    //         {
    //             if (input[i] == '"')
    //             {
    //                 tokens.push(Token(T_STR, token));
    //                 isTagOpened = false;
    //                 expectedTokenType = T_NULL;
    //                 token = "";
    //                 continue;
    //             }
    //             else
    //             {
    //                 token += input[i];
    //                 continue;
    //             }
    //         }

    //         // Parse int
    //         if (isNumber(input[i]) && expectedTokenType == T_NULL)
    //         {
    //             expectedTokenType = T_INT;
    //         }

    //         if (expectedTokenType == T_INT)
    //         {
    //             if (isNumber(input[i]))
    //             {
    //                 token += input[i];
    //             }
    //             if (isNumber(input[i + 1]))
    //             {
    //                 continue;
    //             }
    //             else
    //             {
    //                 expectedTokenType = T_NULL;
    //                 tokens.push(Token(T_INT, token));
    //                 token = "";
    //                 continue;
    //             }
    //         }

    //         // Parse characters
    //         switch(input[i])
    //         {
    //             case '"':
    //                 expectedTokenType = T_STR;
    //                 isTagOpened = true;
    //                 break;
    //             case '(':
    //                 tokens.push(Token(T_LPAREN));
    //                 break;
    //             case ')':
    //                 tokens.push(Token(T_RPAREN));
    //                 break;
    //             case '*':
    //                 tokens.push(Token(T_MUL));
    //                 break;
    //             case '/':
    //                 tokens.push(Token(T_ADD));
    //                 break;
    //             case '+':
    //                 tokens.push(Token(T_ADD));
    //                 break;
    //             case '-':
    //                 tokens.push(Token(T_SUB));
    //                 break;
    //         }
    //     }
    // }

    // cout << tokens.size() << endl;


#ifndef LEXER_H
#define LEXER_H

// Reserved
#define T_NONE 0
#define T_NULL 1
#define T_COMMANDNEND 2
#define T_EOF 3
#define T_COMMENT 4

// Data types
#define T_BOOL 10
#define T_INT 11
#define T_FLOAT 12
#define T_STR 13
#define T_VAR 14
#define T_FUNC 15

// Operators
#define T_OPR 20
#define T_BINOP 21

// Keywords
#define T_IF 30
#define T_FOR 31
#define T_WHILE 32
#define T_END 39

// Separators
#define T_LPAREN 40
#define T_RPAREN 41

#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

#include "token.h"
#include "exceptions/exception.h"
#include "exceptions/syntaxError.h"

using namespace std;

class Lexer
{
    private:
        queue<Token> tokens;
        stack<char> openedTags;
        int currentTokenType;
        string currentTokenValue;
        void resetCurrentToken();
        void pushCurrentToken();
        void popSeparator(char);
        void printException(Exception);
        bool isAlpha(char);
        bool isNumber(char);
    public:
        Lexer();
        void add(char c);
        Token next();
        void pushEOL();
        bool eof();
};

Lexer::Lexer()
{
    this->currentTokenType = 0;
    this->currentTokenValue = "";
}

void Lexer::add(char c)
{
    // !! Temporary code !!
    cout << "CHECK " << c << endl;
    cout << "CURRENT " << currentTokenType << endl;
    cout << "        " << currentTokenValue << endl;

    // Handle String
    if (currentTokenType == T_STR && c != openedTags.top())
    {
        currentTokenValue += c;
        return;
    }

    // Handle Comment
    if (c == '#')
        currentTokenType = T_COMMENT;
    if (currentTokenType == T_COMMENT)
        return;

    if (c == '"' || c == '\'')
    {
        if (currentTokenType != T_STR)
        {
            if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }

            currentTokenType = T_STR;
            openedTags.push(c);
            return;
        }
        else
        {
            tokens.push(Token(currentTokenType, currentTokenValue));
            openedTags.pop();
            resetCurrentToken();
            return;
        }
    }

    // Handle symbols
    switch (c)
    {
        case '(':
        {
            if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }
            tokens.push(Token(T_LPAREN, ""));
            return;
        }
        case ')':
        {
            if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }
            tokens.push(Token(T_RPAREN, ""));
            return;
        }
        case '=':
        {
            if (currentTokenType == T_OPR)
            {
                if (currentTokenValue == "=")
                {
                    currentTokenType = T_BINOP;
                    currentTokenValue += "=";
                }
                else
                {
                    // TODO: throw error
                }
                
            }
            else if (currentTokenType == T_BINOP)
            {
                if (currentTokenValue.length() >= 2)
                {
                    printException(SyntaxError(0, 0, "Invalid operator: " + currentTokenValue + c));
                    return;
                }
                switch (currentTokenValue[0])
                {
                    case '<':
                    case '>':
                    case '!':
                    {
                        currentTokenValue += c;
                        return;
                    }
                    default:
                    {
                        // TODO: throw error
                    }
                    return;
                }
            }
            else if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }
            currentTokenType = T_OPR;
            currentTokenValue = "=";
            return;
        }
        case '>':
        case '<':
        {
            if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }
            currentTokenType = T_BINOP;
            currentTokenValue = c;
            return;
        }
        case '!':
        {
            if (currentTokenType == T_BINOP)
            {
                // return error 
            }
            else if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }
            currentTokenType = T_BINOP;
            currentTokenValue = "!";
            return;
        }
        case '*':
        case '/':
        case '+':
        case '-':
        {
            if (currentTokenType != T_NONE)
            {
                pushCurrentToken();
                resetCurrentToken();
            }
            currentTokenValue = c;
            currentTokenType = T_OPR;
            // string temp = "";
            // temp += c;
            // tokens.push(Token(T_OPR, temp));
        }
    }

    // Handle integers
    if (isNumber(c))
    {
        if (currentTokenType == T_INT || currentTokenType == T_FLOAT)
        {
            currentTokenValue += c;
            return;
        }
        if (currentTokenType == T_VAR)
        {
            currentTokenValue += c;
            return;
        }
        if (currentTokenType != T_NONE)
        {
            pushCurrentToken();
            resetCurrentToken();
        }
        currentTokenType = T_INT;
        currentTokenValue += c;
        return;
    }

    // Handle float
    if (currentTokenType == T_FLOAT && c == '.')
    {
        // TODO: add exception
        return;
    }
    
    if (currentTokenType == T_INT)
    {
        currentTokenType = T_FLOAT;
        currentTokenValue += c;
        return;
    }

    // Handle space and keywords
    if (c == ' ')
    {
        if (currentTokenType == T_VAR)
        {
            if (currentTokenValue == "if")
            {
                tokens.push(Token(T_IF, ""));
                resetCurrentToken();
            }
            else if (currentTokenValue == "for")
            {
                tokens.push(Token(T_FOR, ""));
                resetCurrentToken();                
            }
            else if (currentTokenValue == "while")
            {
                tokens.push(Token(T_WHILE, ""));
                resetCurrentToken();               
            }
        }

        if (currentTokenType != T_NONE)
            pushCurrentToken();
        resetCurrentToken();
        return;
    }

    // Handle identifiers
    if (currentTokenType == T_VAR)
    {
        // todo: add identifier validation
        currentTokenValue += c;
        return;
    }

    if (currentTokenType != T_VAR)
    {
        if (currentTokenType != T_NONE) pushCurrentToken();
        resetCurrentToken();
        if (isAlpha(c) || c == '_')
        {
            currentTokenType = T_VAR;
            currentTokenValue = c;
        }
        else
        {
            // TODO: Add parse error exception
        }
        
        return;
    }

    // Handle EOF
    if (c == -1)
    {
        if (currentTokenType != T_NONE)
            pushCurrentToken();
        tokens.push(Token(T_EOF, ""));
    }
}

Token Lexer::next()
{
    if (tokens.empty()) return Token(T_EOF, "");
    
    Token token = tokens.front();
    tokens.pop();
    return token;
}

void Lexer::pushEOL()
{
    if (currentTokenType != T_NONE && currentTokenType != T_COMMENT)
        pushCurrentToken();
    resetCurrentToken();
    tokens.push(Token(T_COMMANDNEND, ""));
}

bool Lexer::eof()
{
    return tokens.empty();
}

void Lexer::resetCurrentToken()
{
    currentTokenType = T_NONE;
    currentTokenValue = "";
    return;
}

void Lexer::pushCurrentToken()
{
    tokens.push(Token(currentTokenType, currentTokenValue));
    return;
}

void Lexer::popSeparator(char c)
{
    if (openedTags.top() == c)
    {
        openedTags.pop();
    }
    else
    {
        // TODO: throw error
    }
    return;
}

void Lexer::printException(Exception e)
{
    cout << e.type << " at line " << e.line << ", col " << e.col << endl
         << e.message << endl;
    exit(1);
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
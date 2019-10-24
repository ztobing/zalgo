#ifndef LEXER_H
#define LEXER_H

// Reserved
#define T_NONE 0
#define T_NULL 1
#define T_COMMANDNEND 2
#define T_EOF 3
#define T_COMMENT 4
#define T_ESC 5

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
#define T_THEN 38
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
        int currentLine, currentCol, currentTokenLine, currentTokenCol;
        void pushCurrentToken();
        void popSeparator(char);
        void setTokenPosition();
        bool isAlpha(char);
        bool isNumber(char);

        bool parseString(char c, string currentLineContent, int line, int col);
        bool parseComment(char c, string currentLineContent, int line, int col);
        bool parseSymbol(char c, string currentLineContent, int line, int col);
        bool parseNumber(char c, string currentLineContent, int line, int col);
        bool parseSpace(char c, string currentLineContent, int line, int col);
        bool parseIdentifier(char c, string currentLineContent, int line, int col);
        bool parseEOF(char c, string currentLineContent, int line, int col);
    public:
        Lexer();
        void add(char c, string currentLineContent, int line, int col);
        Token next();
        Token front();
        void pushEOL(string currentLineContent);
        bool eof();
};

Lexer::Lexer()
{
    this->currentTokenType = 0;
    this->currentTokenValue = "";
    this->currentLine = this->currentCol = this->currentTokenLine = this->currentTokenCol = 1;
}

void Lexer::add(char c, string currentLineContent, int line, int col)
{
    currentLine = line;
    currentCol = col;

    // !! Temporary code !!
    cout << "CHECK " << c << endl;
    cout << "CURRENT " << currentTokenType << endl;
    cout << "        " << currentTokenValue << endl;

    // Process input according to the lexer order of precedence
    if (parseString(c, currentLineContent, line, col)) return;
    if (parseComment(c, currentLineContent, line, col)) return;
    if (parseSymbol(c, currentLineContent, line, col)) return;
    if (parseNumber(c, currentLineContent, line, col)) return;
    if (parseSpace(c, currentLineContent, line, col)) return;
    if (parseIdentifier(c, currentLineContent, line, col)) return;
    if (parseEOF(c, currentLineContent, line, col)) return;

    SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, &"Invalid character: " [c]);
}

bool Lexer::parseString(char c, string currentLineContent, int line, int col)
{
    if (currentTokenType == T_ESC)
    {
        if (c == '"' || c == '\'')
        {
            currentTokenValue += c;
            currentTokenType = T_STR;
            return true;
        }
        SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid operator");
    }
    if (currentTokenType == T_STR && c != openedTags.top())
    {
        if (c == '\\')
        {
            currentTokenType = T_ESC;
            return true;
        }
        currentTokenValue += c;
        return true;
    }
    if (c == '"' || c == '\'')
    {
        if (currentTokenType != T_STR)
        {
            pushCurrentToken();
            currentTokenType = T_STR;
            openedTags.push(c);
            return true;
        }
        else
        {
            pushCurrentToken();
            openedTags.pop();
            return true;
        }
    }
    return false;
}

bool Lexer::parseComment(char c, string currentLineContent, int line, int col)
{
    if (c == '#')
        currentTokenType = T_COMMENT;
    if (currentTokenType == T_COMMENT)
        return true;
    return false;
}

bool Lexer::parseSymbol(char c, string currentLineContent, int line, int col)
{
    switch (c)
    {
        case '(':
        {
            pushCurrentToken();
            currentTokenType = T_LPAREN;
            currentTokenValue = "";
            return true;
        }
        case ')':
        {
            pushCurrentToken();
            currentTokenType = T_RPAREN;
            currentTokenValue = "";
            return true;
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
                    SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
                }
                
            }
            else if (currentTokenType == T_BINOP)
            {
                if (currentTokenValue.length() >= 2)
                {
                    SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
                }
                switch (currentTokenValue[0])
                {
                    case '<':
                    case '>':
                    case '!':
                    {
                        currentTokenValue += c;
                        return true;
                    }
                    default:
                    {
                        SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
                    }
                    return true;
                }
            }
            pushCurrentToken();
            currentTokenType = T_OPR;
            currentTokenValue = "=";
            return true;
        }
        case '>':
        case '<':
        {
            pushCurrentToken();
            currentTokenType = T_BINOP;
            currentTokenValue = c;
            return true;
        }
        case '!':
        {
            if (currentTokenType == T_BINOP)
            {
                // return error 
            }
            pushCurrentToken();
            currentTokenType = T_BINOP;
            currentTokenValue = "!";
            return true;
        }
        case '*':
        case '/':
        case '+':
        case '-':
        {
            pushCurrentToken();
            currentTokenValue = c;
            currentTokenType = T_OPR;
        }
    }
    return false;
}

bool Lexer::parseNumber(char c, string currentLineContent, int line, int col)
{
    // Handle integers
    if (isNumber(c))
    {
        if (currentTokenType == T_INT || currentTokenType == T_FLOAT)
        {
            currentTokenValue += c;
            return true;
        }
        if (currentTokenType == T_VAR)
        {
            currentTokenValue += c;
            return true;
        }
        pushCurrentToken();
        currentTokenType = T_INT;
        currentTokenValue += c;
        return true;
    }

    // Handle float
    if (currentTokenType == T_FLOAT && c == '.')
    {
        // TODO: add exception
        Exception(line, col, currentLineContent, "asd", "asd test");
        return true;
    }
    
    if (c == '.' && currentTokenType == T_INT)
    {
        currentTokenType = T_FLOAT;
        currentTokenValue += c;
        return true;
    }
    return false;
}

bool Lexer::parseSpace (char c, string currentLineValue, int line, int col)
{
    // Handle space and keywords
    if (c == ' ')
    {
        if (currentTokenType == T_VAR)
        {
            if (currentTokenValue == "if")
            {
                currentTokenType = T_IF;
                currentTokenValue = "";
            }
            else if (currentTokenValue == "for")
            {
                currentTokenType = T_FOR;
                currentTokenValue = "";
            }
            else if (currentTokenValue == "while")
            {
                currentTokenType = T_WHILE;
                currentTokenValue = "";
            }
        }
        pushCurrentToken();
        return true;
    }
    return false;
}

bool Lexer::parseIdentifier(char c, string currentLineContent, int line, int col)
{
    // Handle identifiers
    if (currentTokenType == T_VAR)
    {
        // todo: add identifier validation
        if (isAlpha(c) || c == '_' || isNumber(c)) currentTokenValue += c;
        else SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid syntax: " + currentTokenValue + c);
        return true;
    }

    if (currentTokenType != T_VAR)
    {
        if (currentTokenType == T_INT || currentTokenType == T_FLOAT) SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid syntax: " + currentTokenValue + c);

        pushCurrentToken();
        if (isAlpha(c) || c == '_')
        {
            currentTokenType = T_VAR;
            currentTokenValue = c;
        }
        else
        {
            // TODO: Add parse error exception
        }
        
        return true;
    }
    return false;
}

bool Lexer::parseEOF(char c, string currentLineContent, int line, int col)
{
    // Handle EOF
    if (c == -1)
    {
        pushCurrentToken();
        tokens.push(Token(currentTokenLine, currentTokenCol, T_EOF, ""));
        return true;
    }
    return false;
}

Token Lexer::next()
{
    if (tokens.empty()) return Token(currentTokenLine, currentTokenCol, T_EOF, "");
    
    Token token = tokens.front();
    tokens.pop();
    return token;
}

<<<<<<< HEAD
Token Lexer::front()
{
    if (tokens.empty()) return Token(currentTokenLine, currentTokenCol, T_EOF, "");
    return tokens.front();
}

void Lexer::pushEOL()
=======
void Lexer::pushEOL(string currentLineContent)
>>>>>>> 6fbcbe174aa19abf460b462a88073ac36e6ff5b3
{
    if (currentTokenType == T_STR) SyntaxError(currentTokenLine, currentTokenCol, currentLineContent, "Invalid syntax");

    if (currentTokenType != T_NONE && currentTokenType != T_COMMENT)
        pushCurrentToken();
    tokens.push(Token(currentTokenLine, currentTokenCol, T_COMMANDNEND, ""));
}

bool Lexer::eof()
{
    return tokens.empty();
}

// void Lexer::resetCurrentToken()
// {
//     currentTokenType = T_NONE;
//     currentTokenValue = "";
//     return;
// }

void Lexer::pushCurrentToken()
{
    if (currentTokenType != T_NONE)
        tokens.push(Token(currentTokenLine, currentTokenCol, currentTokenType, currentTokenValue));
    currentTokenType = T_NONE;
    currentTokenValue = "";
    setTokenPosition();
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

void Lexer::setTokenPosition()
{
    currentTokenLine = currentLine;
    currentTokenCol = currentCol;
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
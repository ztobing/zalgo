#ifndef LEXER_H
#define LEXER_H

// Reserved
#define T_NONE 0 // Default
#define T_NULL 1 // NULL
#define T_COMMANDNEND 2 // End of line
#define T_EOF 3 // End of file
#define T_COMMENT 4 // #
#define T_ESC 5 // \

// Data types
#define T_BOOL 10 // true, false
#define T_INT 11
#define T_FLOAT 12
#define T_STR 13
#define T_VAR 14
#define T_FUNC 15
#define T_CLASS 16

// Operators
#define T_OPR 20 // +, -, /, *,  >, <, ^, %
#define T_ASSIGN 21 // =, !
#define T_BINCMP 22 // ==, +=, -=, *=, /=, >=, <=, !=, ++, --
#define T_BITCMP 23 // &&, ||

// Keywords
#define T_IF 30
#define T_ELSE 31
#define T_FOR 32
#define T_WHILE 33
#define T_DO 34
#define T_PRINT 35
#define T_INPUT 36
#define T_RETURN 37
#define T_THEN 38
#define T_END 39
#define T_IN 40
#define T_TO 41

// Separators
#define T_LPAREN 50 // (
#define T_RPAREN 51 // )
#define T_LBRACKET 52 // [
#define T_RBRACKET 53 // ]
#define T_COMMA 54 // ,

#include <iostream>
#include <deque>
#include <stack>
#include <algorithm>

#include "token.h"
#include "exceptions/exception.h"
#include "exceptions/syntaxError.h"

using namespace std;

class Lexer
{
    private:
        deque<Token> tokens;
        stack<string> openedTags;
        int currentTokenType;
        string currentTokenValue, currentLineContent;
        int currentLine, currentCol, currentTokenLine, currentTokenCol;
        int currentTokenCount;
        void pushCurrentToken();
        bool popSeparator(string);
        void setTokenPosition();
        bool isAlpha(char);
        bool isNumber(char);

        bool parseString(char c, string currentLineContent, int line, int col);
        bool parseComment(char c, string currentLineContent, int line, int col);
        bool parseSymbol(char c, string currentLineContent, int line, int col);
        bool parseNumber(char c, string currentLineContent, int line, int col);
        bool parseSpace(char c, string currentLineContent, int line, int col);
        bool parseIdentifier(char c, string currentLineContent, int line, int col);
        void parseIdentifier();
        bool parseEOF(char c, string currentLineContent, int line, int col);
    public:
        Lexer();
        void add(char c, string currentLineContent, int line, int col);
        Token next();
        Token front();
        void pushEOL();
        bool eof();
};

Lexer::Lexer()
{
    this->currentTokenType = 0;
    this->currentTokenCount = 0;
    this->currentTokenValue = "";
    this->currentLine = this->currentCol = this->currentTokenLine = this->currentTokenCol = 1;
}

void Lexer::add(char c, string currentLineContent, int line, int col)
{
    if (parseEOF(c, currentLineContent, line, col)) return;
    this->currentLineContent = currentLineContent;
    currentLine = line;
    currentCol = col;
    currentTokenCount++;

    // !! Temporary code !!
    // cout << "CHECK " << c << endl;
    // cout << "CURRENT " << currentTokenType << endl;
    // cout << "        " << currentTokenValue << endl;

    // Process input according to the lexer order of precedence
    if (parseString(c, currentLineContent, line, col)) return;
    if (parseComment(c, currentLineContent, line, col)) return;
    if (parseSymbol(c, currentLineContent, line, col)) return;
    if (parseNumber(c, currentLineContent, line, col)) return;
    if (parseSpace(c, currentLineContent, line, col)) return;
    if (parseIdentifier(c, currentLineContent, line, col)) return;
    SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid character: '" + string(1, c) + "'");
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
        SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid escape symbol.");
    }
    if (currentTokenType == T_STR && string(1, c) != openedTags.top())
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
            openedTags.push(string(1, c));
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
            openedTags.push(string(1, c));
            return true;
        }
        case ')':
        {
            pushCurrentToken();
            currentTokenType = T_RPAREN;
            currentTokenValue = "";
            return popSeparator("(");
        }
        case '[':
        {
            pushCurrentToken();
            currentTokenType = T_LBRACKET;
            currentTokenValue = "";
            openedTags.push(string(1, c));
            return true;
        }
        case ']':
        {
            pushCurrentToken();
            currentTokenType = T_RBRACKET;
            currentTokenValue = "";
            return popSeparator("[");
        }
        case ',':
        {
            pushCurrentToken();
            currentTokenType = T_COMMA;
            currentTokenValue = "";
            return true;
        }
        case '=':
        {
            if (currentTokenType == T_OPR || currentTokenType == T_ASSIGN || currentTokenType == T_BINCMP)
            {
                if (currentTokenValue.length() >= 2)
                {
                    SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
                }
                switch (currentTokenValue[0])
                {
                    case '<':
                    case '>':
                    case '!':
                    case '=':
                    {
                        currentTokenType = T_BINCMP;
                        currentTokenValue += "=";
                        return true;
                    }
                    default:
                    {
                        SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
                    }
                }
                SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
            }
            pushCurrentToken();
            currentTokenType = T_ASSIGN;
            currentTokenValue = "=";
            return true;
        }
        case '>':
        case '<':
        {
            pushCurrentToken();
            currentTokenType = T_BINCMP;
            currentTokenValue = c;
            return true;
        }
        case '!':
        {
            pushCurrentToken();
            currentTokenType = T_ASSIGN;
            currentTokenValue = c;
            return true;
        }
        case '%':
        case '^':
        case '*':
        case '/':
        case '+':
        case '-':
        {
            pushCurrentToken();
            currentTokenType = T_OPR;
            currentTokenValue = c;
            return true;
        }
        case '&':
        case '|':
        {
            if (currentTokenType == T_BITCMP)
            {
                string s(1, c);
                if (s == currentTokenValue)
                {
                    currentTokenValue += c;
                    return true;
                }
                SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid operator: " + currentTokenValue + c);
            }
            pushCurrentToken();
            currentTokenValue = c;
            currentTokenType = T_BITCMP;
            return true;
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
        SyntaxError(currentTokenLine, currentCol, currentLineContent, "Only one dot is allowed: " + currentTokenValue + c);
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
        parseIdentifier();
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
        if (isAlpha(c) || c == '_' || isNumber(c)) currentTokenValue += c;
        else SyntaxError(currentTokenLine, currentCol, currentLineContent, "Invalid variable name: " + currentTokenValue + c);
        return true;
    }

    if (currentTokenType != T_VAR)
    {
        if (currentTokenType == T_INT || currentTokenType == T_FLOAT) SyntaxError(currentTokenLine, currentCol, currentLineContent, "Variable name can't begin with a number.");

        if (isAlpha(c) || c == '_')
        {
            pushCurrentToken();
            currentTokenType = T_VAR;
            currentTokenValue = c;
            return true;
        }
    }
    return false;
}

void Lexer:: parseIdentifier()
{
    if (currentTokenType == T_VAR)
    {
        if (currentTokenValue == "if")
        {
            currentTokenType = T_IF;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "else")
        {
            currentTokenType = T_ELSE;
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
        // else if (currentTokenValue == "do")
        // {
        //     currentTokenType = T_DO;
        //     currentTokenValue = "";
        // }
        else if (currentTokenValue == "print")
        {
            currentTokenType = T_PRINT;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "input")
        {
            currentTokenType = T_INPUT;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "return")
        {
            currentTokenType = T_RETURN;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "then")
        {
            currentTokenType = T_THEN;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "end")
        {
            currentTokenType = T_END;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "func" || currentTokenValue == "function")
        {
            currentTokenType = T_FUNC;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "class")
        {
            currentTokenType = T_CLASS;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "and")
        {
            currentTokenType = T_BITCMP;
            currentTokenValue = "&&";
        }
        else if (currentTokenValue == "or")
        {
            currentTokenType = T_BITCMP;
            currentTokenValue = "||";
        }
        else if (currentTokenValue == "is")
        {
            currentTokenType = T_ASSIGN;
            currentTokenValue = "=";
        }
        else if (currentTokenValue == "equals")
        {
            currentTokenType = T_BINCMP;
            if (tokens.back().value == "!") currentTokenValue = "!=";
            else currentTokenValue = "==";
        }
        else if (currentTokenValue == "not")
        {
            currentTokenType = T_ASSIGN;
            currentTokenValue = "!";
        }
        else if (currentTokenValue == "true")
        {
            currentTokenType = T_BOOL;
            currentTokenValue = "1";
        }
        else if (currentTokenValue == "false")
        {
            currentTokenType = T_BOOL;
            currentTokenValue = "0";
        }
        else if (currentTokenValue == "in")
        {
            currentTokenType = T_IN;
            currentTokenValue = "";
        }
        else if (currentTokenValue == "to")
        {
            currentTokenType = T_TO;
            currentTokenValue = "";
        }
    }
}

bool Lexer::parseEOF(char c, string currentLineContent, int line, int col)
{
    // Handle EOF
    if (c == -1)
    {
        if (!openedTags.empty())
        {
            string s;
            if (openedTags.top() == "(")
            {
                s = ")";
            }
            else if (openedTags.top() == "[")
            {
                s = "]";
            }
            else if (openedTags.top() == "then")
            {
                s = "end";
            }
            else if (openedTags.top() == "end")
            {
                openedTags.pop();
                openedTags.pop();
                s = openedTags.top();
            }
            else
            {
                s = "then";
            }
            SyntaxError(tokens.back().line, tokens.back().col, tokens.back().lineContent, "Expected: " + s);
        }
        pushCurrentToken();
        tokens.push_back(Token(currentTokenLine, currentTokenCol, currentLineContent, T_EOF, ""));
        return true;
    }
    return false;
}

Token Lexer::next()
{
    if (tokens.empty()) return Token(currentTokenLine, currentTokenCol, currentLineContent, T_EOF, "");
    
    Token token = tokens.front();
    tokens.pop_front();
    return token;
}

Token Lexer::front()
{
    if (tokens.empty()) return Token(currentTokenLine, currentTokenCol, currentLineContent, T_EOF, "");
    return tokens.front();
}

void Lexer::pushEOL()
{
    if (currentTokenType == T_STR) SyntaxError(currentTokenLine, currentCol, currentLineContent, "\" is missing!");

    if (currentTokenType != T_NONE)
        parseIdentifier();
        pushCurrentToken();
    if (tokens.size() != currentTokenCount) tokens.push_back(Token(currentTokenLine, currentTokenCol, currentLineContent, T_COMMANDNEND, ""));
    currentTokenCount = tokens.size();
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
        tokens.push_back(Token(currentTokenLine, currentTokenCol, currentLineContent, currentTokenType, currentTokenValue));
    currentTokenType = T_NONE;
    currentTokenValue = "";
    setTokenPosition();
    return;
}

bool Lexer::popSeparator(string c)
{
    if (openedTags.top() == c)
    {
        openedTags.pop();
        return true;
    }
    SyntaxError(currentTokenLine, currentCol, currentLineContent, "Error does not match: " + currentTokenValue + c);
    return false;
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
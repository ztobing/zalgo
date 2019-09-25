#ifndef LEXER_H
#define LEXER_H

// Define tokens
#define T_NULL "NULL"

#define T_STR "STR"
#define T_INT "INT"
#define T_FLOAT "FLOAT"

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
#define T_NEWLINE "NEWLINE"
#define T_EOF "EOF"

#include <iostream>
#include <queue>
#include <deque>
#include <fstream>
#include <sstream>

#include "token.h"
#include "exceptions/exception.h"
#include "exceptions/syntaxErrorException.h"

using namespace std;

class Lexer
{
    private:
        queue<Token> tokens;
        bool isAlpha(char c);
        bool isNumber(char c);
        void throwException(Exception e);
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
    string inputLine;
    string token;
    string expectedTokenType = T_NULL;

    // String tokenizer variables
    bool isTagOpened = false;

    while (getline(fStream, inputLine))
    {
        stringstream ss(inputLine);
        string input;
        deque<Token> currentLineTokens;

        while (!ss.eof())
        {
            ss >> input;

            if (input == "if")
            {
                if (currentLineTokens.size() == 0) currentLineTokens.push_back(Token(T_IF));
                else throwException(SyntaxErrorException(0, 0)); // TODO: add line and col numbers
                continue;
            }
            else if (input == "else")
            {
                currentLineTokens.push_back(Token(T_ELSE));
                continue;
            }
            else if (input == "for")
            {
                currentLineTokens.push_back(Token(T_FOR));
                continue;
            }
            else if (input == "while")
            {
                currentLineTokens.push_back(Token(T_WHILE));
                continue;
            }

            if (isAlpha(input[0]))  // If it detect alphabet first in line
            {
                if (((currentLineTokens.size() != 0) ? currentLineTokens.back().tokenType : T_NULL) != T_VAR)
                    currentLineTokens.push_back(Token(T_VAR, input));   // Parse as variable
                else throwException(SyntaxErrorException(0, 0)); // TODO: add line and col numbers
                continue;
            }
            else if (isNumber(input[0]))    // If it detect integer first in line
            {
                if (input[1] == '.')    // Check if its a float by checking if the char after the first is .
                {
                    currentLineTokens.push_back(Token(T_FLOAT, input));     // Parse as float
                    continue;
                }
                else
                {
                    bool ST00PID = false;
                    for (int i = 0; i < input.size(); i++)
                    {
                        if (isAlpha(input[i]))  // if it found alphabet after integer that mean UR ST00PID 
                        {
                            ST00PID = true;
                            break;
                        }
                    }
                    if (!ST00PID)   // If smart boi
                    {
                        currentLineTokens.push_back(Token(T_INT, input));   // Parse as int
                    }
                }
            }
        }

        // Empty validated tokens
        while (!currentLineTokens.empty())
        {
            tokens.push(currentLineTokens.front());
            currentLineTokens.pop_front();
        }
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

void Lexer::throwException(Exception e)
{
    cout << "Exception happened on runtime: " << e.exception << endl << e.description << endl;
    exit(1);
}

#endif
#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <queue>
#include <deque>
#include <fstream>

#include "token.h"

using namespace std;

#define T_NULL "NULL"
#define T_NEWLINE "NEWLINE"
#define T_INCOMPATIBLE "asdfghjkl"

// Datatypes
#define T_INT "INT"
#define T_FLAOT "FLOAT"
#define T_STR "STR"
#define T_BOOL "BOOL"

// Containers
#define T_ARR "ARR"
#define T_MAP "MAP"

// Keywords
#define T_KWORD "KWORD"
#define T_VAR "VAR"
#define T_FUNC "FUNC"

// stuff
#define T_OPR "OPR"


class Lexer
{
    private:
        queue<Token> tokens;
        bool isSeparator(char);
        string getType(char);
        string compareType(string, string);
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
    ifstream ifs(filePath);

    // Validation
    if (!ifs.good())
    {
        cout << "Can't read file: " << filePath << endl;
        exit(1);
    }

    // Tokenizer variables
    string currentLine;
    string currentToken = "";
    string currentTokenType = T_NULL;

    while (getline(ifs, currentLine))
    {
        queue<Token> currentLineTokens;
        // TODO: Add string tokenizer

        // Check every character in a line
        for (int i = 0; i < currentLine.length(); i++)
        {
            char c = currentLine[i];
            cout << "-- CHECK " << c << " --" << endl;
            if (isSeparator(c))
            {
                // Add current token to the queue
                if (currentTokenType != T_NULL) currentLineTokens.push(Token(currentTokenType, currentToken));

                cout << "SEP DETECTED" << endl;
                cout << "PUSHEDTOKEN " << currentTokenType << " " << currentToken << endl;

                // Add currently selected character
                switch(c)
                {
                    case ' ':
                        cout << "CURRENTSEP SPACE" << endl;
                        break;
                    case '=':
                        currentLineTokens.push(Token(T_OPR, "="));
                        cout << "CURRENTSEP EQUALS" << endl;
                        break;
                }
                
                // Clear current token data
                currentToken = "";
                currentTokenType = T_NULL;
            }
            else
            {
                // Compare current token with the token before
                if ((currentTokenType = compareType(currentTokenType, getType(c))) == T_INCOMPATIBLE)
                {
                    cout << "InvalidTokenAsdfghjkl" << endl;
                    exit(1);
                }

                currentToken += c;
                cout << "CURRENTTOKEN " << currentToken << endl;
            }

            // endLineCheck
            if (i == currentLine.length())
            {
                cout << "EO"
            }
        }

        // TEST
        currentToken = "";
        currentTokenType = T_NULL;

        // Append tokens after reading one line
        while (!currentLineTokens.empty())
        {
            tokens.push(currentLineTokens.front());
            currentLineTokens.pop();
        }

        tokens.push(Token(T_NEWLINE));
    }

    // For debugging purposes
    while (!tokens.empty())
    {
        Token currentToken = tokens.front();
        cout << "Token(" << currentToken.tokenType << ", " << currentToken.value << ")" << endl;
        tokens.pop();
    }
}

bool Lexer::isSeparator(char c)
{
    // isSeparator
    //
    // Checks if a character is a valid separator
    // Input: Character that will be checked
    // Output: Boolean that represents if a character is a valid separator.

    // TODO: Optimize this shit
    
    char separators[] 
    {',', '=', '(', ')', '[', ']', '"'};
    for (char currentChar : separators)
        if (currentChar == c) return true;

    return false;
}

string Lexer::getType(char c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) return T_VAR;
    else if (c >= 48 && c <= 57) return T_INT;
    // else if (c == '+')
    else if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) ||
             (c >= 123 && c <= 126)) return "SYM";
    else return "CHAR";
}

string Lexer::compareType(string lastToken, string currentToken)
{
    // compareType
    //
    // Checks if the last character's expected token type is compatible with the current 
    // character's expected token type.
    // Input: last known expected token, current expected token type
    
    // For debugging purposes
    cout << "TOKENCMP " << lastToken << " " << currentToken << endl;

    // Allow T_NULL as first tokeb
    if (lastToken == T_NULL) return currentToken;

    // Check integer
    if (lastToken == T_INT)
    {
        if (currentToken == T_INT) return T_INT;
        // else if () TODO: validate decimals
        else return T_INCOMPATIBLE;
    }

    // Check variable definition
    if (lastToken == T_VAR)
    {
        if (currentToken == T_VAR) return T_VAR;
    }
    return T_INCOMPATIBLE;
}

#endif
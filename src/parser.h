#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <queue>

#include "token.h"
#include "lexer.h"
#include "ast.h"

using namespace std;

class Parser
{
    private:
        Lexer lexer;
        Token currentToken;
        void eat(int tokenType);

        void expr();
    public:
        Parser(Lexer);
        void genAST();
};

Parser::Parser(Lexer lexer)
{
    this->lexer = lexer;
    this->currentToken = lexer.next();
}

void Parser::eat(int tokenType)
{
    if (tokenType == currentToken.type) this->currentToken = lexer.next();
    else {cout << "error gobs" << endl; exit(1);}
}

void Parser::expr()
{
    Token token = this->currentToken;
    if (token.type == T_INT)
    {
        
    }
}

#endif
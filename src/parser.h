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
        bool eat(int);

        AST expr();
        AST term();
        AST factor();
    public:
        Parser();
        Parser(Lexer);
        AST genAST();
};

Parser::Parser() {}

Parser::Parser(Lexer lexer)
{
    this->lexer = lexer;
    this->currentToken = this->lexer.next();
}

AST Parser::genAST()
{
    AST result = expr();
    AST temp = result;
    while (temp.right != NULL)
    {
        cout << temp.right->value << endl;
        temp = *temp.right;
    }
    return result;
}

bool Parser::eat(int type)
{
    if (this->currentToken.type == type)
    {
        this->currentToken = this->lexer.next();
        return true;
    }
    return false;
}

AST Parser::expr()
{
    Token token = currentToken;
    AST exprNode = term();
    while (currentToken.type == T_OPR && (currentToken.value == "+" || currentToken.value == "-"))
    {
        Token token = currentToken;
        if (!eat(T_OPR)); // Throw exception
        AST newExprNode(token.type, token.value);
        newExprNode.left = new AST(exprNode);
        newExprNode.right = new AST(term());
        exprNode = newExprNode;
    }
    return exprNode;
}

AST Parser::term()
{
    AST termNode = factor();
    while (currentToken.type == T_OPR && (currentToken.value == "*" || currentToken.value == "/"))
    {
        Token token = currentToken;
        if (!eat(T_OPR)); // Throw exception
        AST newTermNode(token.type, token.value);
        newTermNode.left = new AST(termNode);
        newTermNode.right = new AST(factor());
        termNode = newTermNode;
    }
    return termNode;
}

AST Parser::factor()
{
    Token token = currentToken;
    cout << "Factor: " << currentToken.value << endl;
    // Unary plus/minus
    if (currentToken.type == T_OPR && (currentToken.value == "+" || currentToken.value == "-"))
    {
        if (!eat(T_OPR)); // Throw exception
        AST ast(T_OPR, token.value);
        ast.left = new AST(factor());
        return ast;
    }
    // Integer
    if (currentToken.type == T_INT)
    {
        if (!eat(T_INT)); // Throw exception
        AST ast(T_INT, token.value);
        return ast;
    }
    // Float
    if (currentToken.type == T_FLOAT)
    {
        if (!eat(T_FLOAT)); // Throw exception
        AST ast(T_INT, token.value);
        return ast;
    }
    // Parentheses
    if (currentToken.type == T_LPAREN)
    {
        if (!eat(T_LPAREN)); // Throw exception
        AST ast = expr();
        if (!eat(T_RPAREN)); // Throw exception
        return ast;
    }
    // Variable
    if (currentToken.type == T_VAR)
    {
        if (!eat(T_VAR)); // Throw exception
        AST ast(T_VAR, token.value);
        return ast;
    }
    // Unknown
    // Throw exception
}

#endif
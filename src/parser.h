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

        AST factor();
        AST term();
        AST expr();
    public:
        Parser(Lexer);
        void genAST();
};

Parser::Parser(Lexer lexer)
{
    this->lexer = lexer;
    this->currentToken = this->lexer.next();
}

void Parser::genAST()
{
    AST result = expr();
    cout << result.value.value << endl;
    while (result.right != NULL)
    {
        cout << "Visiting " << result.value.value << endl;
        result = *result.left;
    }
}

void Parser::eat(int tokenType)
{
    cout << "Eating " << currentToken.type << " cmp " << tokenType << endl;
    if (tokenType == currentToken.type)
    {
        this->currentToken = this->lexer.next();
    }
    else {cout << "error gobs" << endl; exit(1);}
}

AST Parser::factor()
{
    cout << "Fact " << currentToken.value << endl;
    Token token = currentToken;

    if (token.type == T_OPR && token.value == "+")
    {
        eat(T_OPR);
        AST ast(token);
        ast.setLeft(factor());
        return ast;
    }
    if (token.type == T_OPR && token.value == "-")
    {
        eat(T_OPR);
        AST ast(token);
        ast.setLeft(factor());
        return ast;
    }
    if (token.type == T_INT)
    {
        eat(T_INT);
        cout << "factor ate some int shit" << endl;
        return AST(token);
    }
    if (token.type == T_FLOAT)
    {
        eat(T_INT);
        return AST(token);
    }
    if (token.type == T_LPAREN)
    {
        eat(T_LPAREN);
        AST ast = expr();
        eat(T_RPAREN);
        return ast;
    }
    return AST(Token(T_NONE, -1, -1, "fucked " + currentToken.value + to_string(currentToken.type)));
}

AST Parser::term()
{
    cout << "Term " << currentToken.value << endl;
    AST node = factor();

    while (currentToken.type == T_OPR && (currentToken.value == "*" || currentToken.value == "/"))
    {
        Token token = currentToken;
        eat(T_OPR);
        cout << "term ate some shit" << endl;
        AST newNode(token);
        newNode.setLeft(node);
        newNode.setRight(factor());
        node = newNode;
    }
    cout << "term done" << endl;
    return node;
}

AST Parser::expr()
{
    cout << "Expr " << currentToken.value << endl;
    AST node = term();

    while (currentToken.type == T_OPR && (currentToken.value == "+" || currentToken.value == "-"))
    {
        Token token = currentToken;
        eat(T_OPR);
        cout << "expr ate some shit" << endl;
        AST newNode(token);
        newNode.setLeft(node);
        newNode.setRight(term());
        node = newNode;
    }
    cout << "expr done" << endl;
    return node;
}

#endif
#ifndef PARSER_H
#define PARSER_H

#define P_NOMATCH 100
#define P_STATEMENT 101
#define P_STATEMENTLIST 102
#define P_FUNCTION 103

#include <iostream>

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
        bool peek(int);

        AST program();
        AST statementList();
        AST statement();
        AST assignStatement();
        AST compareStatement();
        AST expr();
        AST term();
        AST factor();
        AST variable();
        AST function();
        AST string();
    public:
        Parser();
        Parser(Lexer);
        AST genAST();
        void printInOrder(AST);
};

Parser::Parser() {}

Parser::Parser(Lexer lexer)
{
    this->lexer = lexer;
    this->currentToken = this->lexer.next();
}

AST Parser::genAST()
{
    AST result = program();
    AST temp = result;
    return result;
}

void Parser::printInOrder(AST ast)
{
    if (ast.left != NULL) printInOrder(*ast.left);
    cout << ast.value << endl;
    if (ast.right != NULL) printInOrder(*ast.right);
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

bool Parser::peek(int type)
{
    if (this->currentToken.type == type) return true;
    return false;
}

AST Parser::program()
{
    // program
    // statement_list EOF
    cout << "program START" << endl;

    AST programNode = statementList();
    if (!eat(T_EOF)) cout << "EOF NOT FOUND" << endl; // Throw exception
    return programNode;
}

AST Parser::statementList()
{
    // statement_list
    // statement NEWLINE statement_list 
    cout << "statement_list START" << endl;

    AST statementListNode = statement();

    while (currentToken.type == T_COMMANDNEND)
    {
        eat(T_COMMANDNEND);
        AST newStatementListNode(P_STATEMENTLIST, "");
        newStatementListNode.left = new AST(statementListNode);
        AST rightNode = statementList();
        if (rightNode.type == P_NOMATCH) return statementListNode;
        newStatementListNode.right = new AST(rightNode);
        statementListNode = newStatementListNode;
    }
    cout << "statement_list END" << endl;
    return statementListNode;
}

AST Parser::statement()
{
    // statement
    // block_statement | assignment_statement | compare_statement | call_statement | empty
    cout << "statement START" << endl;

    AST statementNode(P_STATEMENT, "");

    // Block Statement


    // Assignment Statement
    statementNode = assignStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // Call Statement

    // Compare Statement
    // statementNode = compareStatement();
    // if (statementNode.type != P_NOMATCH) return statementNode;

    // Empty Statement

    // Unknown
    cout << "statement END" << endl;
    return AST(P_NOMATCH, "");
}

AST Parser::assignStatement()
{
    // assign_statement
    // variable ASSIGN expr | function variable LPAREN (variable (COMMA variable)*)* RPAREN NEWLINE 
    cout << "assignStatement START" << endl;

    // Variable definition
    if (currentToken.type == T_VAR)
    {
        Token varToken = currentToken;
        if (!eat(T_VAR)) return AST(P_NOMATCH, "");
        AST assignNode(T_ASSIGN, "=");
        if (!peek(T_ASSIGN)) { currentToken = varToken; return AST(P_NOMATCH, ""); }
        eat(T_ASSIGN);
        assignNode.left = new AST(T_VAR, varToken.value);
        assignNode.right = new AST(expr());
        cout << "assignStatement END" << endl;
        return assignNode;
    }
    // Function definition
    if (currentToken.type == T_FUNC)
    {
        eat(T_FUNC);
        Token funcToken = currentToken;
        eat(T_VAR);
        AST functionNode(P_FUNCTION, funcToken.value);
        if (!eat(T_LPAREN)); // Throw exception
        if (peek(T_VAR))
        {
            // Add parameter processing
        }
        if (!eat(T_RPAREN)); // Throw exception
        if (!eat(T_COMMANDNEND)); // Throw exception
        // functionNode.left = new AST(); // Add parameter list
        functionNode.right = new AST(statementList());
        if (!eat(T_END)) cout << "END NOT FOUND" << endl; // Throw exception
        if (!eat(T_FUNC)) cout << "END_FUNC NOT FOUND" << endl; // Throw exception
        return functionNode;
    }
    return AST(P_NOMATCH, "");
}

AST Parser::compareStatement()
{
    // compare_statement
    // expr COMPARE expr

    AST leftExpr = expr();
    Token compareToken = currentToken;
    if (!eat(T_BINCMP) || !eat(T_BITCMP)) return AST(P_NOMATCH, ""); // Throw exception
    AST rightExpr = expr();
    AST compareAST(compareToken.type, compareToken.value);
    compareAST.left = new AST(leftExpr);
    compareAST.right = new AST(rightExpr);
    return compareAST;
}

AST Parser::expr()
{
    // expr
    // term ((ADD | SUB) term)*
    cout << "expr START" << endl;

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
    cout << "expr END" << endl;
    return exprNode;
}

AST Parser::term()
{
    // term
    // factor ((MUL | DIV) factor)*
    cout << "term START" << endl;

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
    cout << "term END" << endl;
    return termNode;
}

AST Parser::factor()
{
    // factor
    // PLUS factor | MIN factor | INT | LPAREN expr RPAREN | variable | string

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
        return variable();
    }
    // String
    if (currentToken.type == T_STR)
    {
        return string();
    }
    // Unknown
    // Throw exception
}

AST Parser::variable()
{
    // variable
    // T_VAR

    Token token = currentToken;
    if (!eat(T_VAR)) return AST(P_NOMATCH, "");
    AST variableNode(T_VAR, token.value);
    return variableNode;
}

AST Parser::function()
{
    // function
    // T_FUNC

    Token token = currentToken;
    if (!eat(T_FUNC)) return AST(P_NOMATCH, "");
    AST functionNode(T_VAR, token.value);
    return functionNode;
}

AST Parser::string()
{
    // function
    // T_FUNC

    Token token = currentToken;
    if (!eat(T_STR)) return AST(P_NOMATCH, "");
    AST stringNode(T_STR, token.value);
    return stringNode;
}

#endif
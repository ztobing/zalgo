#ifndef PARSER_H
#define PARSER_H

#define P_NOMATCH 100
#define P_STATEMENT 101
#define P_STATEMENTLIST 102
#define P_EXPRLIST 103
#define P_ARRAY 104
#define P_FUNCTION 105
#define P_IFACTIONS 106

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
        AST exprList();
        AST statement();
        AST assignStatement();
        AST ifStatement();
        AST arrayStatement();
        AST forStatement();
        AST whileStatement();
        AST printStatement();
        AST compareStatement();
        AST expr();
        AST term();
        AST factor();
        AST variable();
        AST function();
        AST string();
        AST input();
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
    // cout << "program START" << endl;

    AST programNode = statementList();
    if (!eat(T_EOF)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: 'eof'"); // Throw exception
    return programNode;
}

AST Parser::statementList()
{
    // statement_list
    // statement NEWLINE statement_list 
    // cout << "statement_list START" << endl;

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
    // cout << "statement_list END" << endl;
    return statementListNode;
}

AST Parser::exprList()
{
    // expr_list
    // (expr (COMMA, expr)*)*

    AST exprListNode = expr();

    while (currentToken.type == T_COMMA)
    {
        eat(T_COMMA);
        AST newExprListNode(P_EXPRLIST, "");
        newExprListNode.left = new AST(exprListNode);
        AST rightNode = exprList();
        if (rightNode.type == P_NOMATCH) return exprListNode;
        newExprListNode.right = new AST(rightNode);
        exprListNode = newExprListNode;
    }

    return exprListNode;
}

AST Parser::statement()
{
    // statement
    // if_statement | for_statement | while_statement | assignment_statement | compare_statement | call_statement | print_statement empty
    // cout << "statement START" << endl;

    AST statementNode(P_STATEMENT, "");

    // If Statement
    statementNode = ifStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // Array statement
    statementNode = arrayStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // For Statement
    // statementNode = forStatement();
    // if (statementNode.type != P_NOMATCH) return statementNode;

    // While Statement
    statementNode = whileStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // Assignment Statement
    statementNode = assignStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // Call Statement

    // Print Statement
    statementNode = printStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // Compare Statement
    statementNode = compareStatement();
    if (statementNode.type != P_NOMATCH) return statementNode;

    // Empty Statement

    // Unknown
    // cout << "statement END" << endl;
    return AST(P_NOMATCH, "");
}

AST Parser::assignStatement()
{
    // assign_statement
    // variable ASSIGN expr | function variable LPAREN (variable (COMMA variable)*)* RPAREN NEWLINE 
    // cout << "assignStatement START" << endl;

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
        // cout << "assignStatement END" << endl;
        return assignNode;
    }
    // Function definition
    if (currentToken.type == T_FUNC)
    {
        eat(T_FUNC);
        Token funcToken = currentToken;
        eat(T_VAR);
        AST functionNode(P_FUNCTION, funcToken.value);
        if (!eat(T_LPAREN)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: '('"); // Throw exception
        if (peek(T_VAR))
        {
            // Add parameter processing
        }
        if (!eat(T_RPAREN)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: ')'"); // Throw exception
        if (!eat(T_COMMANDNEND))SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: 'eol'"); // Throw exception
        // functionNode.left = new AST(); // Add parameter list
        functionNode.right = new AST(statementList());
        if (!eat(T_END)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: 'end'"); // Throw exception
        if (!eat(T_FUNC)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: 'func'"); // Throw exception
        return functionNode;
    }
    return AST(P_NOMATCH, "");
}

AST Parser::ifStatement()
{
    // if_statement
    // IF compare_statement THEN statement_list* (ELSE if_statement)* END IF
    if (!eat(T_IF)) return AST(P_NOMATCH, "");
    AST compareNode = compareStatement();
    if (compareNode.type == P_NOMATCH) return AST(P_NOMATCH, "");
    if (!eat(T_THEN)) return AST(P_NOMATCH, "");
    if (!eat(T_COMMANDNEND)) return AST(P_NOMATCH, "");
    AST ifNode(T_IF, "");
    ifNode.left = new AST(compareNode);
    ifNode.right = new AST(P_IFACTIONS, "");
    ifNode.right->left = new AST(statementList());

    while (currentToken.type == T_ELSE)
    {
        eat(T_ELSE);
        if (eat(T_IF))
        {
            AST newIfNode(T_IF, "");
            newIfNode.left = new AST(compareStatement());
            if (!eat(T_THEN)) AST(P_NOMATCH, "");
            if (!eat(T_COMMANDNEND)) AST(P_NOMATCH, "");
            AST ifActionNode(P_IFACTIONS, "");
            ifActionNode.left = new AST(statementList());
            newIfNode.right = new AST(ifActionNode);

            // Assign to deepest node
            AST* currentNode = ifNode.right;
            while (currentNode->right != NULL)
            {
                currentNode = currentNode->right;
            }
            currentNode->right = new AST(newIfNode);
            
        }
        else
        {
            if (!eat(T_COMMANDNEND)) AST(P_NOMATCH, "");
            AST elseNode(statementList());

            // Assign to deepest node
            AST* currentNode = ifNode.right;
            while (currentNode->right != NULL)
            {
                currentNode = currentNode->right;
            }
            currentNode->right = new AST(elseNode);
            break;
        }
    }
    if (!eat(T_END)) return AST(P_NOMATCH, "");
    if (!eat(T_IF)) return AST(P_NOMATCH, "");
    return ifNode;
}

AST Parser::arrayStatement()
{
    if (!eat(T_LBRACKET)) return AST(P_NOMATCH, "");
    AST arrayNode(P_ARRAY, "");
    arrayNode.left = new AST(exprList());
    if (!eat(T_RBRACKET)) return AST(P_NOMATCH, "");
    return arrayNode;
}

AST Parser::forStatement()
{
    
}

AST Parser::whileStatement()
{
    if (!eat(T_WHILE)) return AST(P_NOMATCH, "");
    AST whileNode(T_WHILE, "");
    whileNode.left = new AST(compareStatement());
    if (!eat(T_THEN)) return AST(P_NOMATCH, "");
    whileNode.right = new AST(statementList());
    if (!eat(T_END)) return AST(P_NOMATCH, "");
    if (!eat(T_WHILE)) return AST(P_NOMATCH, "");
    return whileNode;
}

AST Parser::compareStatement()
{
    // compare_statement
    // expr COMPARE expr

    AST leftExpr = expr();
    Token compareToken = currentToken;
    if (!(eat(T_BINCMP) || eat(T_BITCMP))) return AST(P_NOMATCH, "");
    AST rightExpr = expr();
    AST compareAST(compareToken.type, compareToken.value);
    compareAST.left = new AST(leftExpr);
    compareAST.right = new AST(rightExpr);
    return compareAST;
}

AST Parser::printStatement()
{
    // print_statement
    // print expr

    if (!eat(T_PRINT)) return AST(P_NOMATCH, "");
    AST printNode(T_PRINT, "");
    printNode.left = new AST(expr());
    return printNode;
}

AST Parser::expr()
{
    // expr
    // term ((ADD | SUB) term)*
    // cout << "expr START" << endl;

    Token token = currentToken;
    AST exprNode = term();
    while (currentToken.type == T_OPR && (currentToken.value == "+" || currentToken.value == "-"))
    {
        Token token = currentToken;
        if (!eat(T_OPR))SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: '" + currentToken.value + "'"); // Throw exception
        AST newExprNode(token.type, token.value);
        newExprNode.left = new AST(exprNode);
        newExprNode.right = new AST(term());
        exprNode = newExprNode;
    }
    // cout << "expr END" << endl;
    return exprNode;
}

AST Parser::term()
{
    // term
    // factor ((MUL | DIV) factor)*
    // cout << "term START" << endl;

    AST termNode = factor();
    while (currentToken.type == T_OPR && (currentToken.value == "*" || currentToken.value == "/"))
    {
        Token token = currentToken;
        if (!eat(T_OPR)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: '" + currentToken.value + "'"); // Throw exception
        AST newTermNode(token.type, token.value);
        newTermNode.left = new AST(termNode);
        newTermNode.right = new AST(factor());
        termNode = newTermNode;
    }
    // cout << "term END" << endl;
    return termNode;
}

AST Parser::factor()
{
    // factor
    // PLUS factor | MIN factor | INT | LPAREN expr RPAREN | variable | string

    Token token = currentToken;
    // cout << "Factor: " << currentToken.value << endl;
    // Unary plus/minus
    if (currentToken.type == T_OPR && (currentToken.value == "+" || currentToken.value == "-"))
    {
        if (!eat(T_OPR)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: '" + currentToken.value + "'"); // Throw exception
        AST ast(T_OPR, token.value);
        ast.left = new AST(factor());
        return ast;
    }
    // Integer
    if (currentToken.type == T_INT)
    {
        if (!eat(T_INT))SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: 'int'"); // Throw exception
        AST ast(T_INT, token.value);
        return ast;
    }
    // Float
    if (currentToken.type == T_FLOAT)
    {
        if (!eat(T_FLOAT)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: 'float'"); // Throw exception
        AST ast(T_FLOAT, token.value);
        return ast;
    }
    // Parentheses
    if (currentToken.type == T_LPAREN)
    {
        if (!eat(T_LPAREN)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: '('"); // Throw exception
        AST ast = expr();
        if (!eat(T_RPAREN)) SyntaxError(currentToken.line, currentToken.col, currentToken.lineContent, "Expected: ')'"); // Throw exception
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
    // Input
    if (currentToken.type == T_INPUT)
    {
        return input();
    }
    // Array
    if (currentToken.type == P_ARRAY)
    {
        return arrayStatement();
    }

    return AST(P_NOMATCH, "");
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

AST Parser::input()
{
    eat(T_INPUT);
    AST inputNode(T_INPUT, "");
    inputNode.left = new AST(expr());
    return inputNode;
}

#endif
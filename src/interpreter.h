#ifndef INTERPRETER_H
#define INTERPRETER_H

#define I_NOMATCH 300
#define I_COMPLETE 301

#include <iostream>
#include <map>
#include <math.h>

#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "value.h"

class Interpreter
{
    private:
        AST ast;
        map<string, Value> GLOBAL_SCOPE;
        Value visit(AST);
        Value visitStatementList(AST);
        Value visitAssign(AST);
        Value visitOpr(AST);
        Value visitVar(AST);
    public:
        Interpreter(AST);
        void interpret();
};

Interpreter::Interpreter(AST ast)
{
    this->ast = ast;
}

void Interpreter::interpret()
{
    visit(this->ast);
    cout << "GLOBAL SCOPE" << endl;
    for (map<string, Value>::iterator itr =  GLOBAL_SCOPE.begin(); itr != GLOBAL_SCOPE.end(); itr++)
    {
        cout << itr->first << "\t" << itr->second.type << "\t" << itr->second.value << endl; 
    }
}

Value Interpreter::visit(AST ast)
{
    // Preorder tasks
    // if (ast.left != NULL) visit(*ast.left);
    // Value preorderVal = ast.left != NULL ? visit(*ast.left) : Value(I_NOMATCH, "");

    // Inorder tasks
    cout << "Current: " << ast.value << endl;

    switch (ast.type)
    {
        case P_STATEMENTLIST:
            return visitStatementList(ast);
        case T_ASSIGN:
            return visitAssign(ast);
        case T_OPR:
            return visitOpr(ast);
        default:
            break;
    }

    // Postorder tasks
    // if (ast.right != NULL) visit(*ast.right);
    return Value(P_NOMATCH, "");
}

Value Interpreter::visitStatementList(AST ast)
{
    visit(*ast.left);
    return visit(*ast.right);
}

Value Interpreter::visitAssign(AST ast)
{
    string identifier = ast.left->value;
    Value rightHandValue = visit(*ast.right);
    if (rightHandValue.type == I_NOMATCH)
    ; // Throw exception
    GLOBAL_SCOPE[identifier] =  rightHandValue;
    return Value(I_COMPLETE, "");
}

Value Interpreter::visitOpr(AST ast)
{
    // --- BASE CASE --- //
    if ((ast.type == T_INT || ast.type == T_FLOAT) && ast.left == nullptr && ast.right == nullptr)
    {
        return Value(ast.type, ast.value);
    }

    // --- UNARY --- //
    if ((ast.value == "-" || ast.value == "+") && ast.right == NULL)
    {
        if (ast.left->type == T_INT)
        {
            if (ast.value == "-") return Value(T_INT, to_string(-stoi(visitOpr(*ast.left).value)));
            if (ast.value == "+") return Value(T_INT, to_string(+stoi(visitOpr(*ast.left).value)));
        }
        else if (ast.left->type == T_FLOAT)
        {
            if (ast.value == "-") return Value(T_FLOAT, to_string(-stod(visitOpr(*ast.left).value)));
            if (ast.value == "+") return Value(T_FLOAT, to_string(+stod(visitOpr(*ast.left).value)));
        }
    }

    // --- ADD --- //
    if (ast.value == "+")
    {
        Value leftHand, rightHand;
        leftHand = ast.left->type == T_OPR ? visitOpr(*ast.left) : Value(ast.left->type, ast.left->value);
        rightHand = ast.right->type == T_OPR ? visitOpr(*ast.right) : Value(ast.right->type, ast.right->value);
        // Integer
        if (leftHand.type == T_INT && rightHand.type == T_INT)
        {
            return Value(T_INT, to_string(stoi(leftHand.value) + stoi(rightHand.value)));
        }
        // Float
        if (leftHand.type == T_FLOAT || rightHand.type == T_FLOAT)
        {
            return Value(T_FLOAT, to_string(stod(leftHand.value) + stod(rightHand.value)));
        }
        // String
        if (leftHand.type == T_STR || rightHand.type == T_STR)
        {
            return Value(T_STR, leftHand.value + rightHand.value);
        }
    }

    // --- SUB --- //
    if (ast.value == "-")
    {
        Value leftHand, rightHand;
        leftHand = ast.left->type == T_OPR ? visitOpr(*ast.left) : Value(ast.left->type, ast.left->value);
        rightHand = ast.right->type == T_OPR ? visitOpr(*ast.right) : Value(ast.right->type, ast.right->value);
        // Integer
        if (leftHand.type == T_INT && rightHand.type == T_INT)
        {
            return Value(T_INT, to_string(stoi(leftHand.value) - stoi(rightHand.value)));
        }
        // Float
        if (leftHand.type == T_FLOAT || rightHand.type == T_FLOAT)
        {
            return Value(T_FLOAT, to_string(stod(leftHand.value) - stod(rightHand.value)));
        }
        // String
        if (leftHand.type == T_STR || rightHand.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
    }

    // --- MUL --- //
    if (ast.value == "*")
    {
        Value leftHand, rightHand;
        leftHand = ast.left->type == T_OPR ? visitOpr(*ast.left) : Value(ast.left->type, ast.left->value);
        rightHand = ast.right->type == T_OPR ? visitOpr(*ast.right) : Value(ast.right->type, ast.right->value);
        // Integer
        if (leftHand.type == T_INT && rightHand.type == T_INT)
        {
            return Value(T_INT, to_string(stoi(leftHand.value) * stoi(rightHand.value)));
        }
        // Float
        if (leftHand.type == T_FLOAT || rightHand.type == T_FLOAT)
        {
            return Value(T_FLOAT, to_string(stod(leftHand.value) * stod(rightHand.value)));
        }
        // String
        if (leftHand.type == T_STR || rightHand.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
    }

    // --- DIV --- //
    if (ast.value == "/")
    {
        Value leftHand, rightHand;
        leftHand = ast.left->type == T_OPR ? visitOpr(*ast.left) : Value(ast.left->type, ast.left->value);
        rightHand = ast.right->type == T_OPR ? visitOpr(*ast.right) : Value(ast.right->type, ast.right->value);
        // Integer
        if (leftHand.type == T_INT && rightHand.type == T_INT)
        {
            return Value(T_INT, to_string(stoi(leftHand.value) / stoi(rightHand.value)));
        }
        // Float
        if (leftHand.type == T_FLOAT || rightHand.type == T_FLOAT)
        {
            return Value(T_FLOAT, to_string(stod(leftHand.value) / stod(rightHand.value)));
        }
        // String
        if (leftHand.type == T_STR || rightHand.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
    }

    // --- POW --- //
    if (ast.value == "^")
    {
        Value leftHand, rightHand;
        leftHand = ast.left->type == T_OPR ? visitOpr(*ast.left) : Value(ast.left->type, ast.left->value);
        rightHand = ast.right->type == T_OPR ? visitOpr(*ast.right) : Value(ast.right->type, ast.right->value);
        // Integer
        if (leftHand.type == T_INT && rightHand.type == T_INT)
        {
            return Value(T_INT, to_string(pow(stoi(leftHand.value), stoi(rightHand.value))));
        }
        // Float
        if (leftHand.type == T_FLOAT || rightHand.type == T_FLOAT)
        {
            return Value(T_FLOAT, to_string(pow(stod(leftHand.value), stod(rightHand.value))));
        }
        // String
        if (leftHand.type == T_STR || rightHand.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
    }
    return Value(I_NOMATCH, "");
}

Value Interpreter::visitVar(AST ast)
{
    if (ast.type != T_VAR)
    ; // Throw exception
    if (GLOBAL_SCOPE.find(ast.value) == GLOBAL_SCOPE.end())
    ; // Throw exception
    return GLOBAL_SCOPE[ast.value];
}

#endif
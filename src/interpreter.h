#ifndef INTERPRETER_H
#define INTERPRETER_H

#define I_NOMATCH 200
#define I_NONE 201
#define I_COMPLETE 202

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
        Value visitInt(AST);
        Value visitFloat(AST);
        Value visitString(AST);
        Value visitPrint(AST);
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
    // cout << "Current: " << ast.value << " " << ast.type << endl;

    switch (ast.type)
    {
        case P_STATEMENTLIST:   return visitStatementList(ast);
        case T_ASSIGN:          return visitAssign(ast);
        case T_OPR:             return visitOpr(ast);
        case T_INT:             return visitInt(ast);
        case T_FLOAT:           return visitFloat(ast);
        case T_VAR:             return visitVar(ast);
        case T_STR:             return visitString(ast);
        case T_PRINT:           return visitPrint(ast);
        default:                break;
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
    // Process the deepest node before processing
    Value lhs = ast.left ? visit(*ast.left) : Value(I_NONE, "");
    Value rhs = ast.right ? visit(*ast.right) : Value(I_NONE, "");
    if (lhs.type == I_NONE || rhs.type == I_NONE)
    {
        cout << "NONE" << endl;
    }

    if (!(lhs.type == T_INT || lhs.type == T_FLOAT || lhs.type == T_STR)) return Value(I_NOMATCH, "");
    if (!(rhs.type == T_INT || rhs.type == T_FLOAT || rhs.type == T_STR)) return Value(I_NOMATCH, "");

    // --- ADD --- //
    if (ast.value == "+")
    {
        if (lhs.type == T_STR || rhs.type == T_STR)
        {
            string result = lhs.value + rhs.value;
            return Value(T_STR, result);
        }
        if (lhs.type == T_INT && rhs.type == T_INT)
        {
            int result = stoi(lhs.value) + stoi(rhs.value);
            return Value(T_INT, to_string(result));
        }
        if (lhs.type == T_FLOAT || rhs.type == T_FLOAT)
        {
            double result = stod(lhs.value) + stod(rhs.value);
            return Value(T_FLOAT, to_string(result));
        }
        return Value(I_NOMATCH, "");
    }

    // --- MIN --- //
    if (ast.value == "-")
    {
        if (lhs.type == T_STR || rhs.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
        if (lhs.type == T_INT && rhs.type == T_INT)
        {
            int result = stoi(lhs.value) - stoi(rhs.value);
            return Value(T_INT, to_string(result));
        }
        if (lhs.type == T_FLOAT || rhs.type == T_FLOAT)
        {
            int result = stod(lhs.value) - stod(rhs.value);
            return Value(T_FLOAT, to_string(result));
        }
        return Value(I_NOMATCH, "");
    }

    // --- MUL --- //
    if (ast.value == "*")
    {
        if (lhs.type == T_STR && rhs.type == T_INT)
        {
            string result = "";
            for (int i = 0; i < stoi(rhs.value); i++)
            {
                result += lhs.value;
            }
            return Value(T_STR, result);
        }
        if (lhs.type == T_STR || rhs.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
        if (lhs.type == T_INT && rhs.type == T_INT)
        {
            int result = stoi(lhs.value) * stoi(rhs.value);
            return Value(T_INT, to_string(result));
        }
        if (lhs.type == T_FLOAT || rhs.type == T_FLOAT)
        {
            int result = stod(lhs.value) * stod(rhs.value);
            return Value(T_FLOAT, to_string(result));
        }
        return Value(I_NOMATCH, "");
    }

    // --- DIV --- //
    if (ast.value == "/")
    {
        if (lhs.type == T_STR || rhs.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
        if (lhs.type == T_INT && rhs.type == T_INT)
        {
            int result = stoi(lhs.value) / stoi(rhs.value);
            return Value(T_INT, to_string(result));
        }
        if (lhs.type == T_FLOAT || rhs.type == T_FLOAT)
        {
            int result = stod(lhs.value) / stod(rhs.value);
            return Value(T_FLOAT, to_string(result));
        }
        return Value(I_NOMATCH, "");
    }

    // --- POW --- //
    if (ast.value == "^")
    {
        if (lhs.type == T_STR || rhs.type == T_STR)
        {
            return Value(I_NOMATCH, "");
        }
        if (lhs.type == T_INT && rhs.type == T_INT)
        {
            int result = pow(stoi(lhs.value), stoi(rhs.value));
            return Value(T_INT, to_string(result));
        }
        if (lhs.type == T_FLOAT || rhs.type == T_FLOAT)
        {
            int result = pow(stod(lhs.value), stod(rhs.value));
            return Value(T_FLOAT, to_string(result));
        }
        return Value(I_NOMATCH, "");
    }

    return Value(I_NOMATCH, "");
}

Value Interpreter::visitVar(AST ast)
{
    if (ast.type != T_VAR)
    ; // Throw exception

    if (GLOBAL_SCOPE.find(ast.value) == GLOBAL_SCOPE.end())
    ; // Throw exception (IMPORTANT)

    return GLOBAL_SCOPE[ast.value];
}

Value Interpreter::visitInt(AST ast)
{
    if (ast.type != T_INT)
    ; // Throw exception

    return Value(T_INT, ast.value);
}

Value Interpreter::visitFloat(AST ast)
{
    if (ast.type != T_FLOAT)
    ; // Throw exception

    return Value(T_FLOAT, ast.value);
}

Value Interpreter::visitString(AST ast)
{
    if (ast.type != T_STR)
    ; // Throw exception

    return Value(T_STR, ast.value);
}

Value Interpreter::visitPrint(AST ast)
{
    if (ast.type != T_PRINT)
    ; // Throw exception
    cout << visit(*ast.left).value << endl;
    return Value(I_COMPLETE, "");
}

#endif
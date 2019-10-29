#ifndef INTERPRETER_H
#define INTERPRETER_H

#define I_NOMATCH 200
#define I_NONE 201
#define I_COMPLETE 202

#include <iostream>
#include <map>
#include <vector>
#include <regex>
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
        Value visitArray(AST);
        Value visitStatementList(AST);
        Value visitExprList(AST);
        Value visitAssign(AST);
        Value visitIf(AST);
        Value visitFor(AST);
        Value visitWhile(AST);
        Value visitCompare(AST);
        Value visitOpr(AST);
        Value visitVar(AST);
        Value visitInt(AST);
        Value visitFloat(AST);
        Value visitString(AST);
        Value visitInput(AST);
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
    switch (ast.type)
    {
        case P_STATEMENTLIST:   return visitStatementList(ast);
        case P_EXPRLIST:        return visitExprList(ast);
        case T_ASSIGN:          return visitAssign(ast);
        case T_IF:              return visitIf(ast);
        case T_FOR:             return visitFor(ast);
        case T_WHILE:           return visitWhile(ast);
        case T_BINCMP:
        case T_BITCMP:          return visitCompare(ast);
        case T_OPR:             return visitOpr(ast);
        case T_INT:             return visitInt(ast);
        case T_FLOAT:           return visitFloat(ast);
        case T_VAR:             return visitVar(ast);
        case T_STR:             return visitString(ast);
        case T_INPUT:           return visitInput(ast);
        case P_ARRAY:           return visitArray(ast);
        case T_PRINT:           return visitPrint(ast);
        default:                break;
    }

    return Value(P_NOMATCH, "");
}

Value Interpreter::visitExprList(AST ast)
{
    vector<Value> exprs;
    if (ast.left) 
    {
        Value expr = visit(*ast.left);
        exprs.push_back(Value(expr.type, expr.value, expr.values));
    }
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

Value Interpreter::visitIf(AST ast)
{
    Value condition = visitCompare(*ast.left);
    if (condition.value == "0")
    {
        if (ast.right->right != nullptr) return visit(*ast.right->right);
        return Value(I_COMPLETE, "");
    }
    return visit(*ast.right->left);
}

Value Interpreter::visitFor(AST ast)
{
    AST condition = *ast.left;
    AST operation = *ast.right;

    if (condition.type == T_TO)
    {
        for (int i = stoi(condition.left->value); i != stoi(condition.right->value); stoi(condition.left->value) < stoi(condition.right->value) ? i++ : i-- )
        {
            GLOBAL_SCOPE[ast.value] = Value(T_INT, to_string(i));
            visit(AST(operation));
        }
        GLOBAL_SCOPE.erase(GLOBAL_SCOPE.find(ast.value));
    }
    return Value(I_COMPLETE, "");
}

Value Interpreter::visitWhile(AST ast)
{
    AST origCompare(*ast.left);
    AST origOperation(*ast.right);

    Value condition = visitCompare(AST(origCompare));
    bool cond = condition.value == "0" ? false : true;
    
    while(cond)
    {
        visit(AST(origOperation));
        condition = visitCompare(AST(origCompare));
        cond = condition.value == "0" ? false : true;
    }
    return Value(I_COMPLETE, "");
}

Value Interpreter::visitCompare(AST ast)
{
    Value lhs = visit(*ast.left);
    Value rhs = visit(*ast.right);

    bool result = false;

    if (ast.value == "==") 
    {
        switch(lhs.type)
        {
            case T_BOOL:
            case T_INT:
            case T_FLOAT:
            {
                double lhsValue = stod(lhs.value);
                double rhsValue;
                switch (rhs.type)
                {
                    case T_INT: rhsValue = stoi(rhs.value); break;
                    case T_FLOAT: rhsValue = stod(rhs.value); break;
                    case T_BOOL: rhsValue = stoi(rhs.value); break;
                    default: break; // THROW ERROR
                }
                result = lhsValue == rhsValue;
                break;
            }
            case T_STR:
            {
                string lhsValue = lhs.value;
                string rhsValue;
                switch (rhs.type)
                {
                    case T_STR: rhsValue = rhs.value; break;
                    default: break; // THROW ERROR
                }
                result = lhsValue == rhsValue;
                break;
            }
        }
    }
    else if (ast.value == "!=") 
    {
        switch(lhs.type)
        {
            case T_BOOL:
            case T_INT:
            case T_FLOAT:
            {
                double lhsValue = stod(lhs.value);
                double rhsValue;
                switch (rhs.type)
                {
                    case T_INT: rhsValue = stoi(rhs.value); break;
                    case T_FLOAT: rhsValue = stod(rhs.value); break;
                    case T_BOOL: rhsValue = stoi(rhs.value); break;
                    default: break; // THROW ERROR
                }
                result = lhsValue != rhsValue;
                break;
            }
            case T_STR:
            {
                string lhsValue = lhs.value;
                string rhsValue;
                switch (rhs.type)
                {
                    case T_STR: rhsValue = rhs.value; break;
                    default: break; // THROW ERROR
                }
                result = lhsValue != rhsValue;
                break;
            }
        }
    }
    else if (ast.value == "<=") 
    {
        switch(lhs.type)
        {
            case T_BOOL:
            case T_INT:
            case T_FLOAT:
            {
                double lhsValue = stod(lhs.value);
                double rhsValue;
                switch (rhs.type)
                {
                    case T_INT: rhsValue = stoi(rhs.value); break;
                    case T_FLOAT: rhsValue = stod(rhs.value); break;
                    case T_BOOL: rhsValue = stoi(rhs.value); break;
                    default: break; // THROW ERROR
                }
                result = lhsValue <= rhsValue;
                break;
            }
            case T_STR:
            {
                // THROW ERROR
                break;
            }
        }
    }
    else if (ast.value == ">=") 
    {
        switch(lhs.type)
        {
            case T_BOOL:
            case T_INT:
            case T_FLOAT:
            {
                double lhsValue = stod(lhs.value);
                double rhsValue;
                switch (rhs.type)
                {
                    case T_INT: rhsValue = stoi(rhs.value); break;
                    case T_FLOAT: rhsValue = stod(rhs.value); break;
                    case T_BOOL: rhsValue = stoi(rhs.value); break;
                    default: break; // THROW ERROR
                }
                result = lhsValue >= rhsValue;
                break;
            }
            case T_STR:
            {
                // THROW ERROR
                break;
            }
        }
    }
        else if (ast.value == "<") 
    {
        switch(lhs.type)
        {
            case T_BOOL:
            case T_INT:
            case T_FLOAT:
            {
                double lhsValue = stod(lhs.value);
                double rhsValue;
                switch (rhs.type)
                {
                    case T_INT: rhsValue = stoi(rhs.value); break;
                    case T_FLOAT: rhsValue = stod(rhs.value); break;
                    case T_BOOL: rhsValue = stoi(rhs.value); break;
                    default: break; // THROW ERROR
                }
                result = lhsValue < rhsValue;
                break;
            }
            case T_STR:
            {
                // THROW ERROR
                break;
            }
        }
    }
    else if (ast.value == ">") 
    {
        switch(lhs.type)
        {
            case T_BOOL:
            case T_INT:
            case T_FLOAT:
            {
                double lhsValue = stod(lhs.value);
                double rhsValue;
                switch (rhs.type)
                {
                    case T_INT: rhsValue = stoi(rhs.value); break;
                    case T_FLOAT: rhsValue = stod(rhs.value); break;
                    case T_BOOL: rhsValue = stoi(rhs.value); break;
                    default: break; // THROW ERROR
                }
                result = lhsValue > rhsValue;
                break;
            }
            case T_STR:
            {
                // THROW ERROR
                break;
            }
        }
    }

    // String
    // if (ast.value == "==") result = lhs.value == rhs.value;
    // if (ast.value == ">=") result = lhs.value >= rhs.value;
    // if (ast.value == "<=") result = lhs.value <= rhs.value;
    // if (ast.value == "!=") result = lhs.value != rhs.value;
    // if (ast.value == ">") result = lhs.value > rhs.value;
    // if (ast.value == "<") result = lhs.value < rhs.value;

    return Value(T_BOOL, to_string(result));
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
            double result = stod(lhs.value) - stod(rhs.value);
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
            double result = stod(lhs.value) * stod(rhs.value);
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
            double result = stod(lhs.value) / stod(rhs.value);
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
            double result = pow(stod(lhs.value), stod(rhs.value));
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

Value Interpreter::visitInput(AST ast)
{
    string input;
    string prompt = ast.left->value;
    cout << prompt;
    getline(cin, input);

    // Determine token type
    regex r_INT("[\\d]+");
    regex r_FLOAT("[\\d]+.[\\d]+");
    if (regex_match(input, r_INT)) return Value(T_INT, input);
    else if (regex_match(input, r_FLOAT)) return Value(T_FLOAT, input);
    return Value(T_STR, input);
}

Value Interpreter::visitArray(AST ast)
{
    vector<Value> exprs;
    if (ast.left) 
    {
        exprs.push_back(Value(ast.type, ast.value));
    }
}

Value Interpreter::visitPrint(AST ast)
{
    if (ast.type != T_PRINT)
    ; // Throw exception
    cout << visit(*ast.left).value << endl;
    return Value(I_COMPLETE, "");
}

#endif
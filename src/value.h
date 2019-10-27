#ifndef VALUE_H
#define VALUE_H

#include <iostream>

using namespace std;

struct Value
{
    int type;
    string value;
    Value(int, string);
};

Value::Value(int type, string value)
{
    this->type = type;
    this->value = value;
}

#endif
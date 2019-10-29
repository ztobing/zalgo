#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <vector>

using namespace std;

struct Value
{
    int type;
    string value;
    vector<Value> values;
    Value();
    Value(int, string);
    Value(int, vector<Value>);
    Value(int, string, vector<Value>);
};

Value::Value()
{
    this->type = 0;
    this->value = "";
}

Value::Value(int type, string value)
{
    this->type = type;
    this->value = value;
}

Value::Value(int type, vector<Value> values)
{
    this->type = type;
    this->value = "";
    this->values = values;
}

Value::Value(int type, string value, vector<Value> values)
{
    this->type = type;
    this->value = value;
    this->values = values;
}
#endif
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>

using namespace std;

struct Exception
{
    Exception(int, int, string, string, string);
};

Exception::Exception(int line, int col, string currentLine, string type, string message)
{
    cout << type << " at line " << line << ", col " << col << endl
         << currentLine << endl;

    for (int i = 0; i < col - 1; i++)
    {
        cout << " ";
    }
    cout << "^" << endl;
    cout << message << endl;
    exit(1);
}

#endif
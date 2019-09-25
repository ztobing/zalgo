#include <iostream>
#include <string>
#include "lexer.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        // run in shell mode
    }
    else if (argc == 2)
    {
        string x(argv[1]);
        Lexer l(x);
    }
    else
    {
        cout << "Usage: zalgo [filename]" << endl;
    }
    
}
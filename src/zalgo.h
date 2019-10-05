#ifndef ZALGO_H
#define ZALGO_H

#include <iostream>
#include <fstream>

#include "lexer.h"

using namespace std;

class Zalgo
{
    private:
        Lexer lexer;
    public:
        Zalgo();
        Zalgo(string filePath);
};

Zalgo::Zalgo()
{

}

Zalgo::Zalgo(string filePath)
{
    ifstream ifs(filePath);
    
    if (!ifs.good())
    {
        cerr << "Cannot read file " << filePath << endl;
        exit(1);
    }

    // Split strings into chars and pass it to lexer
    // TODO: add line and col number tracker for error handling purposes
    while (!ifs.eof())
    {
        string currentLine;
        getline(ifs, currentLine);

        for (string::iterator itr = currentLine.begin(); itr != currentLine.end(); itr++)
        {
            lexer.add(*itr);
        }

        // Push end of instruction token
        lexer.pushEOL();
    }

    // Push EOF
    lexer.add(-1);

    // !! Temporary code !!
    // Prints all tokenized code after parsing
    cout << "Token\tValue" << endl;
    while (!lexer.eof())
    {
        Token t = lexer.next();
        cout << t.type << "\t" << t.value << endl;
    }
}

#endif
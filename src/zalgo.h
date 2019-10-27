#ifndef ZALGO_H
#define ZALGO_H

#include <iostream>
#include <fstream>

#include "lexer.h"
#include "parser.h"

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
    int lineNumber = 1, colNumber = 1;
    while (!ifs.eof())
    {
        string currentLine;
        getline(ifs, currentLine);

        for (string::iterator itr = currentLine.begin(); itr != currentLine.end(); itr++)
        {
            lexer.add(*itr, currentLine, lineNumber, colNumber);
            colNumber++;
        }

        // Push end of instruction token
        lexer.pushEOL(currentLine);
        lineNumber++;
        colNumber = 1;
    }

    // Push EOF
    lexer.add(-1, " ", -1, -1);

    // !! Temporary code !!
    // Prints all tokenized code after parsing
    // cout << "Token\tLine,col\tValue" << endl;
    // while (!lexer.eof())
    // {
    //     Token t = lexer.next();
    //     cout << t.type << "\t" << t.line << "," << t.col << "\t\t" << t.value << endl;
    // }

    // !! Temporary code !!
    // Prints the parse tree
    Parser parser(lexer);
    parser.genAST();
}

#endif
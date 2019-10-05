#include <iostream>

#include "zalgo.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        cout << "Usage: zalgo [filename]" << endl;
        exit(1);
    }
    else if (argc == 2)
    {
        Zalgo zalgo(argv[1]);
    }

    return 0;
}
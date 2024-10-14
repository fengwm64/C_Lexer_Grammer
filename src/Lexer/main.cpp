#include "myLex.h"

using namespace std;

int main()
{
    myLex lexer("./Lexer/myScanner.lex", "./Lexer/myScannerCode.c");
    lexer.run();

    return 0;
}
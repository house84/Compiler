#ifndef SCANNER_H
#define SCANNER_H
#include <set>
#include <vector>
#include <string>

using namespace std;

class Scanner
{
public:

    //void LexAnalyzer(string);

    struct tokenStruct
    {
        string tokenType;
        string tokenInstance;
        string line;
        string func;
        int nextState;
        int lineNumber;
        int state;
        bool final;
        bool flag;
        bool global;
        char c;
        string::iterator it;

    };

    vector<tokenStruct> LexAnalyzer(string);
};

#endif //SCANNER_H

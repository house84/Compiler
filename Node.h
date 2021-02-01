#ifndef NODE_H
#define NODE_H
#include "Scanner.h"
#include <list>

struct node
{
    string funcName;
    vector<Scanner::tokenStruct> tokenSet;
    vector<node *> children;
    int level;
};

#endif //NODE_H

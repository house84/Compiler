#ifndef P2_PARSER_H
#define P2_PARSER_H
#include "Scanner.h"
#include "Node.h"
#include <iostream>

class Parser {
public:

    node* aux(vector<Scanner::tokenStruct>);

};

#endif //P2_PARSER_H

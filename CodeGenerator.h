#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

class CodeGenerator {

public:

    void codeGenerator(struct node *root, string filename);

    struct blockStruct
    {
        int varCount;
        int level;
    };
};

#endif

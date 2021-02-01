#ifndef P2_TREE_H
#define P2_TREE_H

#include "Node.h"

class Tree {
public:
    void printPreOrder(node* root);
    void inOrder(struct node* root);
    void Testing(node* root);
    node* decorateTree(struct  node *root);
};

#endif //P2_TREE_H

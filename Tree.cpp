#include "Tree.h"
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

//Return DecorateTree (assign Depths)
node* Tree::decorateTree(struct  node *root)
{
    //Variable for depth
    int size;

    // Stack to store the nodes
    stack<node*> nodes;

    root->level = 0;

    // push the current node onto the stack
    nodes.push(root);


    // loop while the stack is not empty
    while (!nodes.empty()) {

        //Store the current node and pop it from the stack
        node* curr = nodes.top();
        nodes.pop();

        //current node has been travarsed
        if(curr != NULL)
        {

            //Set Get Number of children (0-n)
            size = curr ->children.size()-1;

            //Set Node Depth
            while(size + 1 > 0 )
            {
                //Skip null Children
                if(curr->children.at(size) == NULL)
                {
                    size--;
                    continue;
                }

                //Set Depth
                curr->children.at(size)->level = curr->level + 1;
                size --;
            }

            //Set iterator
            vector<Scanner::tokenStruct>:: iterator is = curr->tokenSet.begin();


            // store all the children of current node from
            // right to left.
            vector<node*>::iterator it = curr->children.end();

            //Push Child Nodes to the stack
            while (it != curr->children.begin()) {
                it--;
                nodes.push(*it);
            }
        }
    }

    return root;
}


//Print Tree PreOrder
void Tree::printPreOrder(struct node *root)
{
    // Stack to store the nodes
    stack<node*> nodes;

    // push the current node onto the stack
    nodes.push(root);

    // loop while the stack is not empty
    while (!nodes.empty()) {

        //Store the current node and pop it from the stack
        node* curr = nodes.top();
        nodes.pop();

        //current node has been travarsed
        if(curr != NULL)
        {
            //Print spacing according to depth
            for(int i = 0; i < curr->level; i++)
            {
                cout << "  ";
            }

            //Output Node Name
            cout << "<" << curr->funcName << ">   ";

            //Set iterator
            vector<Scanner::tokenStruct>:: iterator is = curr->tokenSet.begin();

            //Print tokens
            while(is != curr->tokenSet.end())
            {
                cout << is->tokenInstance << "  ";
                is++;
            }

            // store all the children of current node from
            // right to left.
            vector<node*>::iterator it = curr->children.end();

            //Push Child Nodes to the stack
            while (it != curr->children.begin()) {
                it--;
                nodes.push(*it);
            }

            //Next Line format
            cout << endl;
        }
    }
}


//Print Tree PreOrder
void Tree::Testing(struct node *root) {
    int count = 1;

    // Stack to store the nodes
    stack<node *> nodes;
    stack<node *> kids;

    // push the current node onto the stack
    nodes.push(root);


    // loop while the stack is not empty
    while (!nodes.empty()) {

        //Store the current node and pop it from the stack
        node *curr = nodes.top();
        nodes.pop();

        //current node has been travarsed
        if (curr != NULL) {

            //Print spacing according to depth
            for (int i = 0; i < curr->level; i++) {
                cout << "  ";
            }

            //Output Node Name
            cout << "<" << curr->funcName << ">   ";

            // store all the children of current node from
            // right to left.
            vector<node *>::iterator it = curr->children.end();

            //Push Child Nodes to the stack
            while (it != curr->children.begin()) {
                it--;
                nodes.push(*it);
                kids.push(*it);
            }

            //Next Line format
            cout << endl;

        }
    }
}

    // Function to print the inorder traversal
// of the n-ary tree
void Tree::inOrder(struct node* root)
{

    if (root == NULL)
    {
        return;
    }

    cout << endl;

    // Total children count
    int total = root->children.size();

    // All the children except the last
    if(total > 0 )
    {
        for (int i = 0; i < total - 1; i++)
        {
            inOrder(root->children[i]);

        }

        // Print the current node's data
        cout<< root->funcName << " ";

        // Last child
        inOrder(root->children[total - 1]);
    }
    else
    {
        inOrder(root->children[0]);
    }

}


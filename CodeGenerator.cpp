//
// Created by Nick House on 12/2/2020.
// CS-4280 Program Translation Project
// Project P4
//
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stack>
#include <fstream>
#include "Node.h"
#include "CodeGenerator.h"

using namespace std;

//Function Declarations Check Global and Locals
void push( string local );                      //Push Variables to Local Stack
void pop();                                     //Pop Variables from Local Stack
void setMirrorStack();                          //Set Mirror Local to Local stack
void pushGlobals(string globe);                 //Push Globals to list
void popLocalStack(struct node *root);          //Pop Blocks for Local Scoping
int findLocal(string local);                    //Search values in Local Stack
bool findGlobals(string arg);                   //Search values in Global Stack
bool findVariables(string arg);                 //Search all Created Variables

//Function Declarations Code Generation
void program(struct node* root, ostream& out);  //<program> ->     	start <vars> main <block> stop
void block(struct node* root, ostream& out);    //<block>   ->     	{ <vars> <stats> }
void vars(struct node* root, ostream& out);     //<vars>    ->     	empty | let Identifier :  Integer    <vars>
void expr(struct node *root, ostream& out);     //<expr>    ->     	<N> / <expr>  | <N> * <expr> | <N>
void N(struct node *root, ostream& out);        //<N>       ->     	<A> + <N> | <A> - <N> | <A>
void A(struct node *root, ostream& out);        //<A>       ->     	% <A> |  <R>
void R(struct node *root, ostream& out);        //<R>       ->     	[ <expr> ] | Identifier | Integer
void stats(struct node *root, ostream& out);    //<stats>   ->     	<stat>  <mStat>
void mStat(struct node* root, ostream& out);    //<mStat>   ->     	empty |  <stat>  <mStat>
void stat(struct node* root, ostream& out);     //<stat>   	->     	<in> .  | <out> .  | <block> | <if> .  | <loop> .  | <assign> .
void in(struct node* root, ostream& out);       //<in>      ->     	scanf [ Identifier ]
void OUT(struct node *root, ostream& out);      //<out>     ->     	printf [ <expr> ]
void IF(struct node *root, ostream& out);       //<if>      ->     	if [ <expr> <RO> <expr> ] then <block>
void loop(struct node *root, ostream& out);     //<loop>    ->     	iter [ <expr> <RO> <expr> ]  <block>
void assign(struct node *root, ostream& out);   //<assign>  ->     	Identifier  = <expr>
                                                //<RO>      ->     	=< | => | == | : : (two tokens)

void destroyTargetFile();                       //Destroy Target File in Case of Error

//Block varCount Handler for Local Variable Scoping
static CodeGenerator::blockStruct blockStruct;
stack<CodeGenerator::blockStruct> blockStack;

//Locals and Global Containers
static list<string> AllVariables;               //List Of All Used Variables
static list<string> Globals;                    //List of Global Variables
static stack<string> Locals;                    //Stack for Locals
static stack<string> MirrorStack;               //Mirror of Local Stack for searching
static int LabelCounter = 0;                    //Temporary Label Name Values
static int VarCounter = 0;                      //Temporary Variable Name
typedef enum {VAR, LABEL} nameType;             //Enum For Variable and Label Creation
static char Name[20];                           //Create New name
bool global = true;                             //Global Boolean Check
static string targetFileName;                   //Global String to Delete Target File upon Error

//=================== Functions from P3 to check Static Semantics as P4 Generates Code =========================

//Push arguments to Local stack
void push( string local )
{
    //Push to Local Stack
    Locals.push(local);

    //Update Mirror Stack
    setMirrorStack();

    //Add to List of Program Variables
    if( findVariables(local) == false )
    {
        AllVariables.push_back(local);
    }
}

//Removes Arguments from Local Stack
void pop()
{
    Locals.pop();

    //Set Mirror Stack to new Local
    setMirrorStack();
}

//Set mirror stack used for Searching
void setMirrorStack()
{
    MirrorStack = Locals;
}

//Find first occurrence of variable instance
int findLocal(string local)
{
    int distance = 0;

    //Search Stack
    while( !MirrorStack.empty() )
    {
        if( local == MirrorStack.top() )
        {
            //Set Mirror stack back to original
            setMirrorStack();

            //return distance
            return distance;
        }

        //Pop stack and increase count
        MirrorStack.pop();

        distance++;
    }

    //Set Mirror Stack back to original
    setMirrorStack();

    //Instance not found
    return -1;
}

//push globals to list
void pushGlobals(string globe)
{
    Globals.push_back(globe);

    //Add to List of Program Variables
    if( findVariables(globe) == false )
    {
        AllVariables.push_back(globe);
    }
}

//Search local List for matches
bool findGlobals(string arg)
{
    list<string>::iterator it = Globals.begin();

    //Push Child Nodes to the stack
    while (it != Globals.end()) {

        if(*it == arg)
        {
            return true;
        }

        it++;
    }

    return false;
}

//Search All Variable Stack for Duplicate
bool findVariables(string arg)
{
    list<string>::iterator it = AllVariables.begin();

    while( it != AllVariables.end() )
    {
        if( *it == arg )
        {
            return true;
        }
        it++;
    }

    return false;
}

//Update Local Stack with Proper Variables
void popLocalStack(struct node *root)
{
    //Check Level of Nodes
    if( !blockStack.empty() )
    {

        //Remove <block> from stack after traversed
        while( root->level <= blockStack.top().level )
        {

            for(int i = 0; i < blockStack.top().varCount; i++)
            {
                //Pop all instances of last block
                pop();
            }

            blockStack.pop();
        }
    }
}


//============ Begin single traversal for Code Generation (P4) and Static Semantics from (P3) ==================

//Public Function to Generate Target File
void CodeGenerator::codeGenerator(struct node *root, string targetFile)
{
    //Create ofstream
    ofstream out;

    //Generate Target File
    out.open(targetFile.c_str());

    //Set Global Target Filename
    targetFileName = targetFile;

    //Start Global/Local Assignments and Code Generation
    program(root, out);

    //Close ofstream
    out.close();
}

//Destroy Target File in event of Variable Error
void destroyTargetFile()
{
    remove(targetFileName.c_str());
}

//Create New Temp Variables
static char *newName(nameType what)
{
    if( what == VAR )   //Create New temp
    {
        sprintf(Name, "T%d", VarCounter++);
    }
    else    //Create New Label
    {
        sprintf(Name, "L%d", LabelCounter++);
    }

    AllVariables.push_back(Name);

    return (Name);
}


//Start Processing Root
//<program> -> start <vars> main <block> stop
void program(struct node *root, ostream& out)
{
    vars(root->children.at(0), out);
    global = false;
    block(root->children.at(1), out);
    out << "STOP" << endl;


    //TEMPORARILY COMMENTED **** THIS IS REQUIRED TO INITIALIZE VARIABLES
    list<string>::iterator it = AllVariables.begin();

    //Initialize Variables
    while( it != AllVariables.end() )
    {
        out << *it << "\t0" << endl;
        it++;
    }
}

//<block> -> { <vars> <stats> }
void block(struct node *root, ostream& out)
{
    if(root == NULL )
    {
        return;
    }

    popLocalStack(root);

    //Add level to block
    blockStruct.level = root->level;

    //Set Block varcount
    blockStruct.varCount = 0;

    //push block
    blockStack.push(blockStruct);

    vars(root->children.at(0), out);
    stats(root->children.at(1), out);

    return;
}

//<vars> -> empty | let Identifier :  Integer  <vars>
void vars(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    //Set iterator
    vector<Scanner::tokenStruct>:: iterator is = root->tokenSet.begin();

    string variable;
    int line;

    while(  is != root->tokenSet.end() )
    {
        if( is->tokenType == "INTtk" )
        {
            out << "LOAD\t" << is->tokenInstance << endl;
        }
        if( is->tokenType == "IDtk")
        {
            variable = is->tokenInstance;
            line = is->lineNumber;
        }

        is++;
    }

    //Check if Variables are still being assigned Globally
    if( global == true )
    {
        if(findGlobals(variable) == false)
        {
            pushGlobals(variable);
        }
        else
        {
            //Global Redefinition Error
            cerr << "[ERROR]: Redefinition of Variable: " << variable << " on Line: " << line << " in Global scope" << endl;

            //Delete Incomplete Target File
            destroyTargetFile();

            exit(1);
        }
    }
    else
    {
        //Check that if Variable has been defined it is not already defined locally
        if( blockStack.top().varCount > 0 && findLocal( variable ) < blockStack.top().varCount
        && findLocal(variable) != -1 )
        {
            cerr << "[ERROR]: Redefinition of Variable: " << variable << " on Line: " << line << " in Local scope" << endl;

            //Delete Incomplete Target File
            destroyTargetFile();

            exit(1);
        }
        else    //Push variable to local Stack up VarCount
        {
            push(variable);
            blockStack.top().varCount++;
        }
    }

    out << "STORE\t" << variable << endl;

    vars(root->children.at(0), out);

}

//<expr> -> <N> / <expr>  | <N> * <expr> | <N>
void expr(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    // Pass to N
    if( root->children.size() == 1 )
    {

        N(root->children.at(0), out);

        return;
    }
        //Multiply
    else if( root->tokenSet.at(0).tokenInstance == "*")
    {
        expr(root->children.at(1), out);

        string variable = newName(VAR);

        out << "STORE\t" << variable << endl;

        N(root->children.at(0), out);

        out << "MULT\t" << variable << endl;

        return;
    }
        //Divide
    else if( root->tokenSet.at(0).tokenInstance == "/" )
    {
        expr(root->children.at(1), out);

        string variable = newName(VAR);

        out << "STORE\t" << variable << endl;

        N(root->children.at(0), out);

        out << "DIV\t" << variable << endl;

        return;
    }
}

//<N> -> <A> + <N> | <A> - <N> | <A>
void N(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    // Pass to A
    if(root->children.size() == 1 )
    {
        A(root->children.at(0), out);

        return;
    }
        //Add
    else if( root->tokenSet.at(0).tokenInstance == "+")
    {

        N(root->children.at(1), out);

        string variable = newName(VAR);

        out << "STORE\t" << variable << endl;

        A(root->children.at(0), out);

        out << "ADD\t" << variable << endl;

        return;
    }
        //Subtract
    else if( root->tokenSet.at(0).tokenInstance == "-" )
    {
        N(root->children.at(1), out);

        string variable = newName(VAR);

        out << "STORE\t" << variable << endl;

        A(root->children.at(0), out);

        out << "SUB\t" << variable << endl;

        return;
    }

}

//<A> -> % <A> |  <R>
void A(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    //Pass to R
    if( root->children.at(0)->funcName == "R" )
    {
        R(root->children.at(0), out);
        return;
    }
    else
    {
        A(root->children.at(0), out);
        out << "MULT    -1" << endl;
        return;
    }

}

//<R> -> [ <expr> ] | Identifier | Integer
void R(struct node *root, ostream& out)
{
    if(root == NULL)
    {
        return;
    }

    popLocalStack(root);

    if( root->tokenSet.at(0).tokenType == "IDtk")
    {
        string tokenInstance = root->tokenSet.at(0).tokenInstance;
        int lineNum = root->tokenSet.at(0).lineNumber;

        //Check local stack for Variable
        if( findLocal( tokenInstance) == -1 )
        {
            if(!findGlobals(tokenInstance))
            {
                //Variable not defined locally or globally, Error message
                cerr << "[ERROR]: Undefined Variable: " << tokenInstance << " on Line: " << lineNum << endl;

                //Delete Incomplete Target File
                destroyTargetFile();

                exit(1);
            }
        }

        out << "LOAD\t" << tokenInstance << endl;

        return;
    }
    else if( root->tokenSet.at(0).tokenType == "INTtk")
    {
        out << "LOAD\t" << root->tokenSet.at(0).tokenInstance << endl;

        return;
    }
    else
    {
        expr(root->children.at(0), out);
        return;
    }
}

//<stats> -> <stat>  <mStat>
void stats(struct node *root, ostream& out)
{
    if(root == NULL )
    {
        return;
    }

    popLocalStack(root);

    stat(root->children.at(0), out);

    mStat(root->children.at(1), out);

    return;
}

//<mStat>  -> empty |  <stat>  <mStat>
void mStat(struct node *root, ostream& out)
{
    if(root == NULL )
    {
        return;
    }

    popLocalStack(root);

    stat(root->children.at(0), out);
    mStat(root->children.at(1), out);

    return;
}

//<stat> ->	<in> .  | <OUT> .  | <block> | <if> .  | <loop> .  | <assign> .
void stat(struct node *root, ostream& out)
{

    if(root == NULL )
    {
        return;
    }

    popLocalStack(root);

    vector<string> nonTerm;

    nonTerm.push_back("in");
    nonTerm.push_back("out");
    nonTerm.push_back("block");
    nonTerm.push_back("iF");
    nonTerm.push_back("loop");
    nonTerm.push_back("assign");

    vector<string>::iterator it = nonTerm.begin();

    int i = 1;

    while( it != nonTerm.end() ) {

        if (*it == root->children.at(0)->funcName)
        {
            break;
        }

        i++;
        it++;
    }

    //Select Next Function
    switch (i)
    {
        case 1: //in
        {
            in(root->children.at(0), out);
            return;
        }
        case 2: //out
        {
            OUT(root->children.at(0), out);
            return;
        }
        case 3: //block
        {
            block(root->children.at(0), out);
            return;
        }
        case 4: //if
        {
            IF(root->children.at(0), out);
            return;
        }
        case 5: //loop
        {
            loop(root->children.at(0), out);
            return;
        }
        case 6: //assign
        {
            assign(root->children.at(0), out);
            return;
        }
        default:
            return;
    }
}

//<in> -> scanf [ Identifier ]
void in(struct node *root, ostream& out)
{
    if(root == NULL )
    {
        return;
    }

    popLocalStack(root);

    //Set iterator
    vector<Scanner::tokenStruct>:: iterator is = root->tokenSet.begin();

    while(  is != root->tokenSet.end() )
    {
        if( is->tokenType == "IDtk" )
        {
            //Check if Variable Exists Locally
            if( findLocal(is->tokenInstance) == - 1)
            {
                //Check if Variable Exists Globally
                if( findGlobals( is->tokenInstance) == false )
                {
                    //Variable not defined locally or globally, Error message
                    cerr << "[ERROR]: Undefined Variable: " << is->tokenInstance << " on Line: " << is->lineNumber << endl;

                    //Delete Incomplete Target File
                    destroyTargetFile();

                    exit(1);
                }
            }

            out << "READ\t" << is->tokenInstance << endl;
            break;
        }

        is++;
    }

    return;
}

//<out> ->	printf [ <expr> ]
void OUT(struct node *root, ostream& out) {

    if(root == NULL )
    {
        return;
    }

    popLocalStack(root);

    expr(root->children.at(0), out);

    string variable = newName(VAR);

    out << "STORE\t" << variable << endl;

    out << "WRITE\t" << variable << endl;

}

//<if> -> if [ <expr> <RO> <expr> ] then <block>
void IF(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    expr(root->children.at(2), out);

    string name1 = newName(VAR);

    out << "STORE\t" << name1 << endl;

    expr(root->children.at(0), out);

    out << "SUB\t" << name1 << endl;

    string label1 = newName(LABEL);

    //Check Conditional Operator <RO>
    if( root->children.at(1)->tokenSet.at(0).tokenInstance == ":"
    && root->children.at(1)->tokenSet.at(1).tokenInstance == ":")
    {
        out << "BRZERO\t" << label1 << endl;
    }
    else if(root->children.at(1)->tokenSet.at(0).tokenInstance == "=<")
    {
        out << "BRPOS\t" << label1 << endl;
    }
    else if(root->children.at(1)->tokenSet.at(0).tokenInstance == "=>")
    {
        out << "BRNEG\t" << label1 << endl;
    }
    else if(root->children.at(1)->tokenSet.at(0).tokenInstance == "==")
    {
        out << "BRNEG\t" << label1 << endl;
        out << "BRPOS\t" << label1 << endl;
    }

    block(root->children.at(3), out);

    out << label1 << ":\tNOOP" << endl;
}

//<loop> -> iter [ <expr> <RO> <expr> ]  <block>
void loop(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    string label1 = newName(LABEL);

    out << label1 << ":\t";

    expr(root->children.at(2), out);

    string name1 = newName(VAR);

    out << "STORE\t" << name1 << endl;

    expr(root->children.at(0), out);

    out << "SUB\t" << name1 << endl;

    string label2 = newName(LABEL);

    //Check Conditional Operator <RO>
    if( root->children.at(1)->tokenSet.at(0).tokenInstance == ":"
        && root->children.at(1)->tokenSet.at(1).tokenInstance == ":")
    {
        out << "BRZERO\t" << label2 << endl;
    }
    else if(root->children.at(1)->tokenSet.at(0).tokenInstance == "=<")
    {
        out << "BRPOS\t" << label2 << endl;
    }
    else if(root->children.at(1)->tokenSet.at(0).tokenInstance == "=>")
    {
        out << "BRNEG\t" << label2 << endl;
    }
    else if(root->children.at(1)->tokenSet.at(0).tokenInstance == "==")
    {
        out << "BRNEG\t" << label2 << endl;
        out << "BRPOS\t" << label2 << endl;
    }

    block(root->children.at(3), out);
    out << "BR\t" << label1 << endl;
    out << label2 <<":\t" << "NOOP" << endl;

}

//<assign> -> Identifier  = <expr>
void assign(struct node *root, ostream& out)
{
    if( root == NULL )
    {
        return;
    }

    popLocalStack(root);

    string tokenInstance = root->tokenSet.at(0).tokenInstance;
    int lineNum = root->tokenSet.at(0).lineNumber;

    //Check local stack for Variable
    if( findLocal( tokenInstance) == -1 )
    {
        if(!findGlobals(tokenInstance))
        {
            //Variable not defined locally or globally, Error message
            cerr << "[ERROR]: Undefined Variable: " << tokenInstance << " on Line: " << lineNum << endl;

            //Delete Incomplete Target File
            destroyTargetFile();

            exit(1);
        }
    }

    //Call Child Node
    expr(root->children.at(0), out);
    out << "STORE\t" << root->tokenSet.at(0).tokenInstance << endl;

}


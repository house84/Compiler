#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include "Scanner.h"
#include "Parser.h"
#include "CodeGenerator.h"
#include "Tree.h"

using namespace std;

int main(int argc, const char * argv[])
{
    //**********GET USER INPUT************

    //Create String variables to hold Input and Output file names
    string filename;
    string targetFile;

    //If file name is given open file and display contents
    if(argc==2)
    {
        //Take command line argument and create string
        string comline(argv[1]);
        filename = comline;

        //Create TargetFile Name
        targetFile = comline + ".asm";

        //Concatenate filename with file extension
        filename += ".fs";


        //Delmar uses CPP 4.8, so when using ifstream
        //Do the following to resolve below conflict that
        //will be generated
        ifstream check(filename.c_str());
        if( !check )
        {
            cerr << "[ERROR]: Program file not found" << endl;
            exit(1);
        }
    }


    //No command arguments check for file redirect or needs user input
    else if( argc == 1 )
    {
        //Create string to hold user input
        string input;

        //Open instance of Ofstream and create file;
        ofstream file_write("userinput.fs");
        filename = "userinput.fs";
        targetFile = "kb.asm";

        //Check for keyboard input or redirected file input
        if( isatty(STDIN_FILENO) )
        {
            cout << "Enter File Data: ";
        }

        //Take User input and save to file
        while(getline(cin, input))
        {

            //Write to file
            input += '\n';
            file_write << input;
        }

        //Close file
        file_write.close();
    }

        //Too many arguments throw Err
    else
    {
        cerr << "Invalid Input, to many arguments" << endl;
        exit(1);
    }

    //Create Instance of Scanner
    Scanner scan;

    //Create Vector of Token Struct
    vector<Scanner::tokenStruct> tk;

    //Retrieve Values from Scanner
    tk =  scan.LexAnalyzer(filename);

    //Create Parser Object
    Parser parse;


    //Create node Object
    node* root;

    //Get Root Node for Tree
    root = parse.aux(tk);

    //Testing P4 Outputs
    CodeGenerator CG;
    CG.codeGenerator(root, targetFile);

    //Display Target Name
    cout << "Target name: " << targetFile << endl;


    return 0;
}
//
// Created by Nick House on 10/11/2020.
// CS-4280 Program Translation Project
// Project P1
//
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "Scanner.h"


using namespace std;
Scanner::tokenStruct aStruct;
vector<Scanner::tokenStruct> tk;

//FSA table Logic Enums
enum fsaLogic { OPERATOR = 0, EQUAL = 1, GREATER = 2, LESS = 3,  LOWER = 4, UPPER = 5, DIGIT = 6,
    WS = 7, EOFILE =8 };

//Token Enums
enum tokens { IDtk = 1001, KWtk = 1002, OPtk = 1003, INTtk = 1004, EOFtk = 1005 };

//Error Enums
enum errors { CHARERR = -1 , CAPERR = -2, INTERR = -3 };


//Check identifier string for KW match
int checkKW( string ID )
{
    if( ID == "start" )
    {
        return KWtk;
    }

    if( ID == "stop" )
    {
        return KWtk;
    }

    if( ID == "iter" )
    {
        return KWtk;
    }

    if( ID == "void" )
    {
        return KWtk;
    }

    if( ID == "int" )
    {
        return KWtk;
    }

    if( ID == "exit" )
    {
        return KWtk;
    }

    if( ID == "scanf" )
    {
        return KWtk;
    }

    if( ID == "printf" )
    {
        return KWtk;
    }

    if( ID == "main" )
    {
        return KWtk;
    }

    if( ID == "if" )
    {
        return KWtk;
    }

    if( ID == "then" )
    {
        return KWtk;
    }

    if( ID == "let" )
    {
        return KWtk;
    }

    if( ID == "data" )
    {
        return KWtk;
    }

    if( ID == "func" )
    {
        return KWtk;
    }

    else
    {
        return IDtk;
    }
}

//Check Chars for valid inputs
int checkChar(char ch)
{
    if( ch == '=' )
    {
        return EQUAL;
    }

    if( ch == '>' )
    {
        return GREATER;
    }

    if( ch == '<' )
    {
        return LESS;
    }

    if( ch == ':' )
    {
        return OPERATOR;
    }

    if( ch == '+' )
    {
        return OPERATOR;
    }

    if( ch == '-' )
    {
        return OPERATOR;
    }

    if( ch == '*' )
    {
        return OPERATOR;
    }

    if( ch == '/' )
    {
        return OPERATOR;
    }

    if( ch == '%' )
    {
        return OPERATOR;
    }

    if( ch == '.' )
    {
        return OPERATOR;
    }

    if( ch == '(' )
    {
        return OPERATOR;
    }

    if( ch == ')' )
    {
        return OPERATOR;
    }

    if( ch == ',' )
    {
        return OPERATOR;
    }

    if( ch == '{' )
    {
        return OPERATOR;
    }

    if( ch == '}' )
    {
        return OPERATOR;
    }

    if( ch == ';' )
    {
        return OPERATOR;
    }

    if( ch == '[' )
    {
        return OPERATOR;
    }

    if( ch == ']' )
    {
        return OPERATOR;
    }

    if( ch == EOFtk )
    {
        return EOFILE;
    }

    if( isdigit( ch ))
    {
        return DIGIT;
    }

    if( isspace( ch ))
    {
        return WS;
    }

    if( islower( ch ))
    {
        return LOWER;
    }

    if( isupper( ch ))
    {
        return UPPER;
    }

    if( ch == '\0')
    {
        return WS;
    }

    else
    {
        return CHARERR;
    }
}

//Get Token as a String
string getTokenID(int num)
{
    string value = "NOT DEFINED";

    if(num == 1001)
    {
        return "IDtk";
    }

    if( num == 1002 )
    {
        return "KWtk";
    }

    if( num == 1003 )
    {
        return "OPtk";
    }

    if( num == 1004 )
    {
        return "INTtk";
    }

    if( num == 1005)
    {
        return "EOFtk";
    }

    return value;
}

//FSA Table -> Find if Current string of chars is a known token or not
//             If a token is not return the scanner will continue to 
//             scan chars until a token is identified. 
int fsaTABLE[7][9] = { 
/*Table -> Rows is int from current char, columns is char of look ahead char. 
Columns:  OP(!=)    =       >       <      Lower   Upper   Digits   WS     EOF     */

        { 3,        5,      3,      3,     1,      CAPERR, 2,       0,     EOFtk   },
        { IDtk,     IDtk,   IDtk,   IDtk,  1,      1,      1,       IDtk,  IDtk    },
        { INTtk,    INTtk,  INTtk,  INTtk, INTtk,  INTtk,  2,       INTtk, INTtk   },
        { OPtk,     OPtk,   OPtk,   OPtk,  OPtk,   OPtk,   OPtk,    OPtk,  OPtk    },
        { OPtk,     OPtk,   OPtk,   OPtk,  OPtk,   OPtk,   OPtk,    OPtk,  OPtk    },
        { OPtk,     6,      6,      6,     OPtk,   OPtk,   OPtk,    OPtk,  OPtk    },
        { OPtk,     OPtk,   OPtk,   OPtk,  OPtk,   OPtk,   OPtk,    OPtk,  OPtk    }
};

//Print Results of Token
void displayResult(Scanner::tokenStruct aStruct) {

    cout << "[Token] --> Type: " << aStruct.tokenType << "\t  Value: " << aStruct.tokenInstance << "\t  Line: "
         << aStruct.lineNumber << endl;
}

//Filter Lines and remove comments
string filter( ifstream& read_file )
{
    string line;

    getline( read_file, line);

    //Filter out comments following #
    if(line.find('#') != std::string::npos )
    {
        line = line.erase(line.find('#' ));
        line += '\r';
    }

    return line;
}


//Scan line for Tokens
Scanner::tokenStruct scanner( Scanner::tokenStruct aStruct )
{
    //Hold FSA table returns current and next
    int current = aStruct.state;
    int nextState;

    //tokenInstance
    string tokenInstance;
    string::iterator it;
    it = aStruct.it;
    aStruct.nextState = nextState = fsaTABLE[current][checkChar(*(it+1))];

    //Check for Lexical Error Uppercase
    if(isupper(*it))
    {
        cerr << "[LEXICAL ERROR]:( Line(" << aStruct.lineNumber << ") --> " << *it << " ): Token can not begin with an uppercase letter\n" << endl;
        exit (1);
    }

    //Check for Lexical Error Invalid Char
    if( checkChar(*it) == CHARERR )
    {
        cerr << "[LEXICAL ERROR]:( Line(" << aStruct.lineNumber << ") --> " << *it << " ): Not a valid character\n" << endl;
        exit(1);
    }

    //Populate tokenInstance and iterate state
    aStruct.tokenInstance += *it;
    aStruct.state = nextState;

    //Check for Final State
    if( nextState < 0 || nextState > 1000 )
    {
        //Check for KW
        aStruct.tokenType = getTokenID(nextState);

        //Check IDtk for KW value
        if( nextState == IDtk )
        {
            aStruct.tokenType = getTokenID(checkKW(aStruct.tokenInstance));
        }

        //Check for space
        if( isspace(aStruct.tokenInstance[0]))
        {
            tokenInstance.clear();
        }

        //Create Next Vector Index
        tk.push_back(aStruct);
        aStruct.tokenInstance.clear();

        //Format State to work with space and without space
        if(isspace(*(it)))
        {
            aStruct.nextState = fsaTABLE[0][checkChar(*(it-1))];
        }
        else
        {
            aStruct.nextState = fsaTABLE[0][checkChar(*(it+1))];
        }

        aStruct.final = true;
    }

    return aStruct;
}

//Open File and scan lines for inputs
vector<Scanner::tokenStruct> Scanner::LexAnalyzer(string filename) {

    //Variables Needed
    int lineNumber = 1;
    static string::iterator  it;
    static string ::iterator end;
    string scrubbed;

    //Open File
    ifstream read_file;
    read_file.open( filename.c_str() );

    //Throw Error if filename not opened
    if( !read_file )
    {
        cerr << "Error: File could not be opened." << endl;
        exit(1);
    }

    //Read Lines From File
    while( !read_file.eof() )
    {

        //Get Next Line and Filter
        scrubbed = filter(read_file);

        //Set iterator begin of line
        aStruct.it = scrubbed.begin();
        it = aStruct.it;
        aStruct.line = scrubbed;
        aStruct.lineNumber = lineNumber;
        aStruct.final = false;

        //Set initial State
        aStruct.c = *it;
        aStruct.state = fsaTABLE[0][checkChar(aStruct.c)];


        //Search line for tokens
        while(aStruct.it != scrubbed.end())
        {

            //Call Scanner with aStruct
            aStruct = scanner(aStruct);

            //Trim WS
            if(isspace(aStruct.tokenInstance[0]))
            {
                aStruct.tokenInstance.clear();
            }

            //Reset aStruct when final is returned
            if(aStruct.final == true)
            {
                aStruct.tokenInstance.clear();
                aStruct.state = aStruct.nextState;
                aStruct.final = false;
            }

            aStruct.it++;
        }

        //Track line Numbers
        lineNumber++;
    }

    //Set EOF
    aStruct.tokenInstance = "EOF";
    aStruct.tokenType = "EOFtk";
    aStruct.lineNumber = lineNumber-2;

    //Add new Vector Index
    tk.push_back(aStruct);

    //Close File
    read_file.close();

    //Clean up file
    if(filename == "userinput.fs"){
        remove(filename.c_str());
    }

    return tk;
}

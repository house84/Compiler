#include "Parser.h"
#include "Tree.h"
#include <stdlib.h>
#include <string>
#include <vector>

node* program();
node* block();
node* vars();
node* expr();
node* N();
node* A();
node* R();
node* stats();
node* mStat();
node* stat();
node* in();
node* out();
node* iF();
node* loop();
node* assign();
node* RO();
node* getNode(string);

//static Scanner::tokenStruct tk;
static vector<Scanner::tokenStruct> tk;

//global Static token for all programs to check.
struct Scanner::tokenStruct token;

//Hold tk index
static int i;

//Hold Error Line Number
int a, b, c, d;

//Create New Node
node * getNode(string funcName)
{

    node *newnode = new node;
    newnode->funcName = funcName;

    return newnode;
}

//Get Token
struct Scanner::tokenStruct getToken()
{
    //Increment through Vector
    i++;

    return tk[i];
}

//Auxiliary Function
//void parser(vector<Scanner::tokenStruct> x)
node* Parser::aux(vector<Scanner::tokenStruct> x)
{
    //Create Root Node
    node * root;

    //Set Variable Vector
    tk = x;
    i = -1;

    //Set tk to first program token
    token = getToken();

    //check for empty file
    if ( token.tokenType == "EOFtk" && i == 0 )
    {
        std::cerr << "File is Empty" << std::endl;

        exit(1);
    }

    //Call First BNF Function
    root = program();

    //if End of File return or Error
    if( token.tokenType == "EOFtk" )
    {
        Tree tree;
        return tree.decorateTree(root);    //Return Decorated Tree
    }
    else
    {
        cerr << "[PARSE ERROR]: Line(" << token.lineNumber << "): EOFtk expected" << endl;
        exit(1);
    }
}

node * program() //<program> -> start <vars> main <block> stop
{
    //Create New Node
    node * p = getNode("program");

    if( token.tokenInstance == "start" )
    {
        a = token.lineNumber;                   //Get Line Error

        p->tokenSet.push_back(token);           //Insert Token

        token = getToken();                     //Consume Token and get next

        p->children.push_back(vars());          //Process vars()

        if( token.tokenInstance == "main" )
        {
            b = token.lineNumber;               //Get line Error

            p->tokenSet.push_back(token);       //Insert Token

            token = getToken();                 //Consume Tk

            p->children.push_back(block());     //Process block()

            if( token.tokenInstance == "stop" )
            {
                c = token.lineNumber;           //Get line numb

                p->tokenSet.push_back(token);   //get Token

                token = getToken();             //Consume tk

                return p;
            }
            else
            {
                std::cerr << "[PARSE ERROR]: Line(" << c << "): KWtk stop expected" << endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): KWtk main expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr <<"[PARSE ERROR]: Line(" << token.lineNumber << "): KWtk start expected" << endl;
        exit(1);
    }
}

node* block()  //<block> -> { <vars> <stats> }
{
    node * p = getNode("block");        //Create new Node

    if( token.tokenInstance == "{" )
    {
        a = token.lineNumber;                   //Get Line Num

        p->tokenSet.push_back(token);           //insert token

        token = getToken();                     //consume tk

        p->children.push_back(vars());          //process vars()

        p->children.push_back(stats());         //process stats();

        if(token.tokenInstance == "}" )
        {
            b = token.lineNumber;               //Get Line num

            p->tokenSet.push_back(token);       //insert token

            token = getToken();                 //consume tk

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk } expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): OPtk { expected" << endl;
        exit(1);
    }
}

//<vars> -> empty | let Identifier : Integer <vars>
node* vars()
{
    node * p = getNode("vars");             //Create new Node

    if(token.tokenInstance == "let" ) {

        a = token.lineNumber;                       //Get Line num

        p->tokenSet.push_back(token);               //insert token

        token = getToken();                         //consume token

        if (token.tokenType == "IDtk") {

            b = token.lineNumber;                   //Get line Num

            p->tokenSet.push_back(token);           //insert token

            token = getToken();                     //consume

            if (token.tokenInstance == ":") {

                c = token.lineNumber;               //Get Line Number

                p->tokenSet.push_back(token);       //insert token

                token = getToken();                 //consume tk

                if (token.tokenType == "INTtk") {

                    d = token.lineNumber;           //Get Line Num

                    p->tokenSet.push_back(token);   //insert token

                    token = getToken();             //consume token

                    p->children.push_back(vars());  //process vars();

                    return p;
                }

                else {

                    std::cerr << "[PARSE ERROR]: Line(" << d << "): INTtk expected" << endl;

                    exit(1);
                }
            }

            else {

                std::cerr << "[PARSE ERROR]: Line(" << c << "): OPtk : expected" << endl;

                exit(1);
            }
        }

        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): IDtk expected" << endl;

            exit(1);

        }
    }

    else
    {
        return NULL;
    }
}


node* expr() 	//<expr> -> <N> / <expr>  | <N> * <expr> | <N>
{

    node * p = getNode("expr");        //Create new Node

    p->children.push_back(N());                 //Process N

    if( token.tokenInstance == "/" || token.tokenInstance == "*" )
    {
        a = token.lineNumber;                   //Get Line

        p->tokenSet.push_back(token);           //insert Token

        token = getToken();                     //Consume Tk

        p->children.push_back(expr());          //Process expr()

        return p;
    }
    else
    {
        return p;
    }
}


//<N> -> <A> + <N> | <A> - <N> | <A>
//First(<A> + <N>) = { %, [, Identifier, Integer }
node* N()
{
    node * p = getNode("N");                 //Create new Node

    p->children.push_back(A());	                //Process A()

    if( token.tokenInstance == "+" || token.tokenInstance == "-" )
    {

        p->tokenSet.push_back(token);            //Add Token

        token = getToken();                     //Consume Tk

        p->children.push_back(N());             //Process N()

        return p;
    }
    else
    {
        return p;
    }
}

node* A()  	//	<A> -> % <A> |  <R>
{
    node * p = getNode("A");           //Create new Node

    if( token.tokenInstance == "%" ) 	        // Checking A -> %<A>
    {

        p->tokenSet.push_back(token);           //Add Token

        token = getToken();                     //Consume Token

        p->children.push_back(A());             //Process A()

        return p;
    }
    else if( token.tokenInstance == "[" || token.tokenType == "IDtk" || token.tokenType == "INTtk" )
    {
        a = token.lineNumber;                   //Get Line Number

        p->children.push_back(R());             //Process R()

        return p;
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): IDtk % or [ or Identifier or Integer expected" << endl;
        exit(1);
    }
}

/*
<R> -> [ <expr> ] | Identifier | Integer
First([<expr>]) = { [ } First(Identifier) = { Identifier }	First(Integer) = { Integer }
*/
node* R()
{
    node * p = getNode("R");           //Create new Node

    if( token.tokenInstance == "[")
    {
        p->tokenSet.push_back(token);           //Add Token

        token = getToken();                     //Consume Token

        p->children.push_back(expr());          //Process expr()

        if( token.tokenInstance == "]")
        {
            a = token.lineNumber;               //Get Line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Tk

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << a << "): OPtk ] expected" << endl;
            exit(1);
        }
    }
    else if( token.tokenType == "IDtk" || token.tokenType == "INTtk" )
    {
        b = token.lineNumber;                   //Get Line Num

        p->tokenSet.push_back(token);            //Add Token

        token = getToken();                     //Consume Tk

        return p;
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk [ , IDtk or INTtk expected" << endl;
        exit(1);
    }
}

node* stats()	//<stats> -> <stat>  <mStat>
{
    node * p = getNode("stats");       //Create new Node

    p->children.push_back(stat());              //Process Stat()
    p->children.push_back(mStat());             //Process mStat();

    return p;
}

/*
<mStat> -> empty |  <stat>  <mStat>
First(<stat> <mStat>) = { scanf, printf, if, iter, Identfier, { } 	First(empty) = Follow<mStat> = Follow(<stats>) = { } }
*/
node* mStat()
{
    node * p = getNode("mStat");       //Create new Node

    if( token.tokenInstance == "scanf" || token.tokenInstance == "printf" || token.tokenInstance == "{" ||
        token.tokenInstance == "if" || token.tokenInstance == "iter" || token.tokenType == "IDtk")
    {
        p->children.push_back(stat());          //Process stat();
        p->children.push_back(mStat());         //Process mStat();

        return p;                               //Added**
    }
    else
    {
        return NULL;
    }
}

/*
<stat> -> <in> .  | <out> .  | <block> | <if> .  | <loop> .  | <assign> .
First(<in> .) = { scanf } First(<out>) = { printf } First(<block>) = { { }
First(<if> .) = { if } First(<loop> .) = { iter } First(<assign> .) = { Identifier }
*/
node* stat()
{
    node * p = getNode("stat");        //Create new Node

    if( token.tokenInstance == "scanf" ) 	    //<in> .
    {
        a = token.lineNumber;                   //get line num

        p->children.push_back(in());            //Process in();

        if( token.tokenInstance == ".")
        {
            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            return p;
        }
        else
        {
            std::cerr << ":: ERROR :: OPtk . expected" << endl;
            exit(1);
        }
    }
    else if( token.tokenInstance == "printf" )	//<out> .
    {
        p->children.push_back(out());           //Process out();

        if( token.tokenInstance == ".")
        {
            a = token.lineNumber;               //Get Line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << a << "): OPtk . expected" << endl;
            exit(1);
        }
    }
    else if ( token.tokenInstance == "{" )		//<block>
    {
        p->children.push_back(block());         //Process block();

        return p;
    }
    else if( token.tokenInstance == "if" )		//<if> .
    {
        p->children.push_back(iF());            //Process iF();

        if( token.tokenInstance == ".")
        {
            b = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk . expected" << endl;
            exit(1);
        }
    }
    else if ( token.tokenInstance == "iter")	//<loop> .
    {
        p->children.push_back(loop());          //Process loop();

        if( token.tokenInstance == ".")
        {
            c = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << c << "): OPtk . expected" << endl;
            exit(1);
        }
    }
    else if( token.tokenType == "IDtk")	//<assign> .
    {
        p->children.push_back(assign());        //Process assign();

        if( token.tokenInstance == ".")
        {
            d = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << d << "): OPtk . expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): scanf or printf or { or if or iter or IDtk expected" << endl;
        exit(1);
    }
}

node* in()	//<in> -> scanf [ Identifier ]
{
    node * p = getNode("in");          //Create new Node

    if( token.tokenInstance == "scanf" )
    {
        a = token.lineNumber;                   //get line num

        p->tokenSet.push_back(token);           //Add token

        token = getToken();                     //Consume Token

        if( token.tokenInstance == "[" )
        {
            b = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            if( token.tokenType == "IDtk" )
            {
                c = token.lineNumber;           //get line num

                p->tokenSet.push_back(token);   //Add Token

                token = getToken();             //Consume Token

                if( token.tokenInstance == "]" )
                {
                    d = token.lineNumber;           //get line num

                    p->tokenSet.push_back(token);   //Add Token

                    token = getToken();             //Consume Token

                    return p;
                }
                else
                {
                    std::cerr << "[PARSE ERROR]: Line(" << d << "): OPtk ] expected" << endl;
                    exit(1);
                }
            }
            else
            {
                std::cerr << "[PARSE ERROR]: Line(" << c << "): IDtk expected" << endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk [ expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): KWtk scanf expected" << endl;
        exit(1);
    }
}

node* out()	//<out> -> printf [ <expr> ]
{
    node * p = getNode("out");         //Create new Node

    if( token.tokenInstance == "printf" )
    {
        a = token.lineNumber;                   //get line num

        p->tokenSet.push_back(token);           //Add Token

        token = getToken();                     //Consume Token

        if( token.tokenInstance == "[" )
        {
            b = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            p->children.push_back(expr());      //Process expr();

            if( token.tokenInstance == "]" )
            {
                c = token.lineNumber;           //get line num

                p->tokenSet.push_back(token);   //Add Token

                token = getToken();             //Consume Token

                return p;
            }
            else
            {
                std::cerr << "[PARSE ERROR]: Line(" << c << "): OPtk ] expected" << endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk [ expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): KWtk printf expected" << endl;
        exit(1);
    }

}

node* iF()	//<if> -> if [ <expr> <RO> <expr> ] then <block>
{
    node * p = getNode("iF");          //Create new Node

    if( token.tokenInstance == "if" )
    {
        a = token.lineNumber;                   //get line num

        p->tokenSet.push_back(token);           //Add Token

        token = getToken();                     //Consume Token

        if( token.tokenInstance == "[" )
        {
            b = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);       //Add Token

            token = getToken();                 //Consume Token

            p->children.push_back(expr());      //Process expr();

            p->children.push_back(RO());        //Process RO();

            p->children.push_back(expr());      //Process expr();

            if( token.tokenInstance == "]" )
            {
                c = token.lineNumber;           //get line num

                p->tokenSet.push_back(token);   //Add Token

                token = getToken();             //Consume Token

                if( token.tokenInstance == "then" )
                {
                    d = token.lineNumber;           //get line num

                    p->tokenSet.push_back(token);   //Add Token

                    token = getToken();             //Consume Token

                    p->children.push_back(block()); //process block();

                    return p;
                }
                else
                {
                    std::cerr << "[PARSE ERROR]: Line(" << d << "): KWtk then expected" << endl;
                    exit(1);
                }
            }
            else
            {
                std::cerr << "[PARSE ERROR]: Line(" << c << "): OPtk ] expected" << endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk [ expected" << endl;
            exit(1);
        }

    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): KWtk if expected" << endl;
        exit(1);
    }
}

node* loop()		//<loop> -> iter [ <expr> <RO> <expr> ]  <block>
{
    node * p = getNode("loop");        //Create new Node

    if( token.tokenInstance == "iter" )
    {
        a = token.lineNumber;                   //get line num

        p->tokenSet.push_back(token);           //Add Token

        token = getToken();                     //Consume Token

        if( token.tokenInstance == "[" )
        {
            b = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);        //Add Token

            token = getToken();                 //Consume Token

            p->children.push_back(expr());      //Process expr();

            p->children.push_back(RO());        //Process RO();

            p->children.push_back(expr());      //Process expr();

            if( token.tokenInstance == "]" )
            {
                c = token.lineNumber;           //get line num

                p->tokenSet.push_back(token);    //Add Token

                token = getToken();             //Consume Token

                p->children.push_back(block()); //Process block();

                return p;
            }
            else
            {
                std::cerr << "[PARSE ERROR]: Line(" << c << "): OPtk ] expected" << endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk [ expected" << endl;
            exit(1);
        }

    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): KWtk iter expected" << endl;
        exit(1);
    }
}

node* assign()	//<assign> -> Identifier  = <expr>
{
    node * p = getNode("assign");            //Create new Node

    if( token.tokenType == "IDtk" )
    {
        a = token.lineNumber;                   //get line num

        p->tokenSet.push_back(token);            //Add Token

        token = getToken();                     //Consume Token

        if( token.tokenInstance == "=" )
        {
            b = token.lineNumber;               //get line num

            p->tokenSet.push_back(token);        //Add Token

            token = getToken();                 //Consume Token

            p->children.push_back(expr());      //Process expr();

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk = expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): Identifier token expected" << endl;
        exit(1);
    }
}

node* RO()		//<RO> ->  =<  | =>   |  ==  |   :  :  (two tokens)
{
    node * p = getNode("RO");      //Create new Node

    if( token.tokenInstance == "=<")
    {
        a = token.lineNumber;               //get line num

        p->tokenSet.push_back(token);       //Add Token

        token = getToken();                 //Consume Token

        return p;
    }
    else if( token.tokenInstance == "=>" )
    {
        p->tokenSet.push_back(token);        //Add Token

        token = getToken();                 //Consume Token

        return p;
    }
    else if( token.tokenInstance == "==" )
    {
        p->tokenSet.push_back(token);        //Add Token

        token = getToken();                 //Consume Token

        return p;
    }
    else if( token.tokenInstance == ":" )
    {
        p->tokenSet.push_back(token);        //Add Token

        token = getToken();                 //Consume Token

        if( token.tokenInstance == ":" )
        {
            b = token.lineNumber;           //get line num

            p->tokenSet.push_back(token);    //Add Token

            token = getToken();             //Consume Token

            return p;
        }
        else
        {
            std::cerr << "[PARSE ERROR]: Line(" << b << "): OPtk : expected" << endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "[PARSE ERROR]: Line(" << a << "): Expected OPtk note received:  =<  or =>   or  ==  or  :  :" << endl;
        exit(1);
    }
}
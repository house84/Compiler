:: P4 :: Storage = Local [130 +20 pts] :: Code Generation and Static Semantics (P3) are done simultaneously.
:: P3 :: (THIS HAS BEEN COMBINED W/ P4) Static Semantics - this program is using the Local Support - 2 Option [100 pts]
:: P1 :: Scanner - this uses Option 4 FSA table - [100 pts]

Author: Nick House
Project: P4
Course: CS-4280 Program Translation Project
Directory Name: P4
Included Files: main.cpp, main.h, Scanner.cpp, Scanner.h, Parser.cpp, Parser.h, Tree.cpp, Tree.h, CodeGenerator.cpp
                CodeGenerator.h, Makefile, ReadMe.txt

What this program does:

    This program scans the user given input and it identifies all tokens by type
    (White Space is not required to separate tokens) along with its line number
    and it's specific instance. If a non-listed token or character is found it
    will terminate and display a Lexical Error with type and line number.
    Additionally this program will parses all tokens based on the defined BNF
    (below) and create a node from each function. If there is an error an appropriate
    error will be thrown with a line number and the program will terminate. Once
    successful parsing has been completed code generation and static semantics will
    be checked simultaneously, this program supports local storage and will search,
    assign, and recognize local and global variables within their allowed scope. While
    static semantics are being checked the target file is being generated. If there is
    error generated while checking static semantics an appropriate error message will
    be given and no target file will be created. The target language is VM ACCumulator
    assembly language. How the source code was input to the compiler will determine the
    target file name. If source code "file.fs" was given as file input (./compfs file)
    then the target file will be created with the name "file.asm". If the source code
    was given from the keyboard either by redirect or typed input the target file will
    be created with the name "kb.asm". Upon successful execution of a file the only
    output displayed will be the name of the target file generated.

To run program:

    First Program must be compiled.
    $ make

    Once compiled the program can be executed by using the following
    $ ./compfs

    This program can be executed using three different command line inputs.

    1) Including filename of a file to be used. ex: $ ./compfs filename

    2) Redirect content of a file in the command line. ex: $ ./compfs <filename.fs

    3) Give input through keyboard when program executes. ex: $ ./compfs

Additional Compiler Information:

  Alphabet:
     Includes all english letters (upper and lower), digits, plus the
     operators shown below. All others will generate Lexical Error

  Identifiers: IDtk
     Begin with lower case letter, can continue with any number of
     letters or digits.

  Keywords: KWtk
     start stop iter void int exit scanf print main if then let data func

  Operators: OPtk
    = == => =< : + - * / % . ( ) , { } ; [ ]

  Integers:  INTtk
    any sequence of decimals, no sign, no decimal point.

  Comments:
    begin with '#' and end with end of line

  BNF:
    <program>   ->     	start <vars> main <block> stop
    <block>    	->     	{ <vars> <stats> }
    <vars>     	->     	empty | let Identifier :  Integer    <vars>
    <expr>     	->     	<N> / <expr>  | <N> * <expr> | <N>
    <N>      	->     	<A> + <N> | <A> - <N> | <A>
    <A>       	->     	% <A> |  <R>
    <R>       	->     	[ <expr> ] | Identifier | Integer
    <stats>     ->     	<stat>  <mStat>
    <mStat>   	->     	empty |  <stat>  <mStat>
    <stat>    	->     	<in> .  | <out> .  | <block> | <if> .  | <loop> .  | <assign> .
    <in>       	->     	scanf [ Identifier ]
    <out>       ->     	printf [ <expr> ]
    <if>      	->     	if [ <expr> <RO> <expr> ] then <block>
    <loop>      ->     	iter [ <expr> <RO> <expr> ]  <block>
    <assign>  	->     	Identifier  = <expr>
    <RO>        ->     	=< | => | == | :: (two tokens)

    When variables are assigned using let IDtk : INTtk, the IDtk is also assigned the value of the
    corresponding INTtk.
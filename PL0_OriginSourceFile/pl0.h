#include <stdio.h>

#define norw        11             // no. of reserved words
#define txmax       100            // length of identifier table
#define nmax        14             // max. no. of digits in numbers
#define al          10             // length of identifiers
#define amax        2047           // maximum address
#define levmax      3              // maximum depth of block nesting
#define cxmax       2000           // size of code array

#define nul         0x1
#define ident       0x2
#define number      0x4
#define plus        0x8
#define minus       0x10
#define times       0x20
#define slash       0x40
#define oddsym      0x80
#define eql         0x100
#define neq         0x200
#define lss         0x400
#define leq         0x800
#define gtr         0x1000
#define geq         0x2000
#define lparen      0x4000
#define rparen      0x8000
#define comma       0x10000
#define semicolon   0x20000
#define period      0x40000
#define becomes     0x80000
#define beginsym    0x100000
#define endsym      0x200000
#define ifsym       0x400000
#define thensym     0x800000
#define whilesym    0x1000000
#define dosym       0x2000000
#define callsym     0x4000000
#define constsym    0x8000000
#define varsym      0x10000000
#define procsym     0x20000000

enum object
{
    constant, variable, proc
};

enum fct
{
    lit, opr, lod, sto, cal, Int, jmp, jpc         // functions
};

/*  lit 0, a : load constant a
    opr 0, a : execute operation a
    lod l, a : load variable l, a
    sto l, a : store variable l, a
    cal l, a : call procedure a at level l
    Int 0, a : increment t-register by a
    jmp 0, a : jump to a
    jpc 0, a : jump conditional to a       */
typedef struct
{
    enum fct f;		// function code
    long l; 		// level
    long a; 		// displacement address
} instruction;

char* err_msg[] =
{
/*  0 */    "",
/*  1 */    "Found ':=' when expecting '='.",
/*  2 */    "There must be a number to follow '='.",
/*  3 */    "There must be an '=' to follow the identifier.",
/*  4 */    "There must be an identifier to follow 'const', 'var', or 'procedure'.",
/*  5 */    "Missing ',' or ';'.",
/*  6 */    "Incorrect procedure name.",
/*  7 */    "Statement expected.",
/*  8 */    "Follow the statement is an incorrect symbol.",
/*  9 */    "'.' expected.",
/* 10 */    "';' expected.",
/* 11 */    "Undeclared identifier.",
/* 12 */    "Illegal assignment.",
/* 13 */    "':=' expected.",
/* 14 */    "There must be an identifier to follow the 'call'.",
/* 15 */    "A constant or variable can not be called.",
/* 16 */    "'then' expected.",
/* 17 */    "';' or 'end' expected.",
/* 18 */    "'do' expected.",
/* 19 */    "Incorrect symbol.",
/* 20 */    "Relative operators expected.",
/* 21 */    "Procedure identifier can not be in an expression.",
/* 22 */    "Missing ')'.",
/* 23 */    "The symbol can not be followed by a factor.",
/* 24 */    "The symbol can not be as the beginning of an expression.",
/* 25 */    "",
/* 26 */    "",	
/* 27 */    "",
/* 28 */    "",
/* 29 */    "",
/* 30 */    "",
/* 31 */    "The number is too great.",
/* 32 */    "There are too many levels."
};


char ch;               // last character read
unsigned long sym;     // last symbol read
char id[al+1];         // last identifier read
long num;              // last number read
long cc;               // character count
long ll;               // line length
long kk, err;
long cx;               // code allocation index

char line[81];
char a[al+1];
instruction code[cxmax+1];
char word[norw][al+1];
unsigned long wsym[norw];
unsigned long ssym[256];

char mnemonic[8][3+1];
unsigned long declbegsys, statbegsys, facbegsys;

struct
{
    char name[al+1];
    enum object kind;
    long val;
    long level;
    long addr;
}table[txmax+1];

char infilename[80];
FILE* infile;

// the following variables for block
long dx;		// data allocation index
long lev;		// current depth of block nesting
long tx;		// current table index

// the following array space for interpreter
#define stacksize 50000
long s[stacksize];	// datastore

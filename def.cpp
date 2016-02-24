/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Define
INST    : 全局变量定义
************************************************ */

#include <cstring>

#include "def.h"

//---------------------------------
//-------------Error
//---------------------------------

char* err_msg[34] =        // 错误信息表
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
/* 32 */    "There are too many levels.",
/* 33 */    "';' expected, but 'ELSE' found."
};
long err;                 // 错误计数

//---------------------------------
//-------------Lexer
//---------------------------------

long cc;                  // 列计数
long ll;                  // 行长度
FILE* infile;             // 输入文件
long cx;                  // 代码分配指针
char ch;                  // 读到的最后一个字符
char line[81];            // 行缓冲区
char a[al+1];             // 词法分析器中用于存放正在被被分析的词
long kk;                  // 当前token的长度
char id[al+1];            // 读到的最后一个标识符
char word[norw][al+1];    // 保留字表
unsigned long wsym[norw]; // 每一个保留字对应的symbol类型表
unsigned long ssym[256];  // 每一个符号对应的symbol类型表
unsigned long sym;        // 读到的最后一个token的类型
long num;                 // 读到的最后一个数字

//---------------------------------
//-------------Environment
//---------------------------------

long tx;                  // 符号表指针
tabletype table[txmax+1]; // 符号表
long dx;                  // 数据栈指针
long lev;                 // 当前层级

//---------------------------------
//-------------Generate
//---------------------------------

instruction code[cxmax+1];// 中间代码
char mnemonic[8][3+1];    // 中间代码助记符表

//---------------------------------
//-------------Parse
//---------------------------------

unsigned long declbegsys; // decl开始符号集合
unsigned long statbegsys; // stmt开始符号集合
unsigned long facbegsys;  // factor开始符号集合

//---------------------------------
//-------------Interpretation
//---------------------------------

long s[stacksize];        // 数据栈

//---------------------------------
//-------------Initialization
//---------------------------------

void globalinit()
{
	for(int i=0; i<256; i++)
    {
        ssym[i]=nul;
    }
    
    strcpy(word[0],  "begin     ");
    strcpy(word[1],  "call      ");
    strcpy(word[2],  "const     ");
    strcpy(word[3],  "do        ");
    strcpy(word[4],  "else      ");
    strcpy(word[5],  "end       ");
    strcpy(word[6],  "exit      ");
    strcpy(word[7],  "if        ");
    strcpy(word[8],  "odd       ");
    strcpy(word[9],  "procedure ");
    strcpy(word[10], "then      ");
    strcpy(word[11], "var       ");
    strcpy(word[12], "while     ");
    
    wsym[0]=beginsym;
    wsym[1]=callsym;
    wsym[2]=constsym;
    wsym[3]=dosym;
    wsym[4]=elsesym;
    wsym[5]=endsym;
    wsym[6]=exitsym;
    wsym[7]=ifsym;
    wsym[8]=oddsym;
    wsym[9]=procsym;
    wsym[10]=thensym;
    wsym[11]=varsym;
    wsym[12]=whilesym;

    ssym['+']=plus;
    ssym['-']=minus;
    ssym['*']=times;
    ssym['/']=slash;
    ssym['(']=lparen;
    ssym[')']=rparen;
    ssym['=']=eql;
    ssym[',']=comma;
    ssym['.']=period;
    ssym[';']=semicolon;
  
    strcpy(mnemonic[lit],"LIT");
    strcpy(mnemonic[opr],"OPR");
    strcpy(mnemonic[lod],"LOD");
    strcpy(mnemonic[sto],"STO");
    strcpy(mnemonic[cal],"CAL");
    strcpy(mnemonic[Int],"INT");
    strcpy(mnemonic[jmp],"JMP");
    strcpy(mnemonic[jpc],"JPC");
  
    declbegsys=constsym|varsym|procsym;
    statbegsys=beginsym|callsym|ifsym|whilesym;
    facbegsys=ident|number|lparen;
}
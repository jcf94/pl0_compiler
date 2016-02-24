/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Define
INST    : ȫ�ֱ�������
************************************************ */

#include <cstring>

#include "def.h"

//---------------------------------
//-------------Error
//---------------------------------

char* err_msg[33] =        // ������Ϣ��
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
long err;                 // �������

//---------------------------------
//-------------Lexer
//---------------------------------

long cc;                  // �м���
long ll;                  // �г���
FILE* infile;             // �����ļ�
long cx;                  // �������ָ��
char ch;                  // ���������һ���ַ�
char line[81];            // �л�����
char a[al+1];             // �ʷ������������ڴ�����ڱ��������Ĵ�
long kk;                  // ��ǰtoken�ĳ���
char id[al+1];            // ���������һ����ʶ��
char word[norw][al+1];    // �����ֱ�
unsigned long wsym[norw]; // ÿһ�������ֶ�Ӧ��symbol���ͱ�
unsigned long ssym[256];  // ÿһ�����Ŷ�Ӧ��symbol���ͱ�
unsigned long sym;        // ���������һ��token������
long num;                 // ���������һ������

//---------------------------------
//-------------Environment
//---------------------------------

long tx;                  // ���ű�ָ��
tabletype table[txmax+1]; // ���ű�
long dx;                  // ����ջָ��
long lev;                 // ��ǰ�㼶

//---------------------------------
//-------------Generate
//---------------------------------

instruction code[cxmax+1];// �м����
char mnemonic[8][3+1];    // �м�������Ƿ���

//---------------------------------
//-------------Parse
//---------------------------------

unsigned long declbegsys; // decl��ʼ���ż���
unsigned long statbegsys; // stmt��ʼ���ż���
unsigned long facbegsys;  // factor��ʼ���ż���

//---------------------------------
//-------------Interpretation
//---------------------------------

long s[stacksize];        // ����ջ

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
    strcpy(word[4],  "end       ");
    strcpy(word[5],  "if        ");
    strcpy(word[6],  "odd       ");
    strcpy(word[7],  "procedure ");
    strcpy(word[8],  "then      ");
    strcpy(word[9],  "var       ");
    strcpy(word[10], "while     ");
    strcpy(word[11], "else      ");
    
    wsym[0]=beginsym;
    wsym[1]=callsym;
    wsym[2]=constsym;
    wsym[3]=dosym;
    wsym[4]=endsym;
    wsym[5]=ifsym;
    wsym[6]=oddsym;
    wsym[7]=procsym;
    wsym[8]=thensym;
    wsym[9]=varsym;
    wsym[10]=whilesym;
    wsym[11]=elsesym;

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
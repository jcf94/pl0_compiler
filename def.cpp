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

char* err_msg[emsgsize] = // ������Ϣ��
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
/* 33 */    "';' expected, but 'ELSE' found.",
/* 34 */    "'EXIT' Found, but outside the 'WHILE'.",
/* 35 */    "There must be an '(' to follow the 'WRITE'/'READ'/'ODD'.",
/* 36 */    "Variable identifier expected."
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
unsigned long long wsym[norw]; // ÿһ�������ֶ�Ӧ��symbol���ͱ�
unsigned long long ssym[256];  // ÿһ�����Ŷ�Ӧ��symbol���ͱ�
unsigned long long sym;        // ���������һ��token������
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

unsigned long long declbegsys; // decl��ʼ���ż���
unsigned long long statbegsys; // stmt��ʼ���ż���
unsigned long long simpexpbegsys;// simpexp��ʼ���ż���
unsigned long long termbegsys; // term��ʼ���ż���
unsigned long long facbegsys;  // factor��ʼ���ż���
long exitlist[elsize];    // while�е�exit��ַ��
long elx;                 // exitlistָ��

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
    
    strcpy(word[0],  "and       ");
    strcpy(word[1],  "begin     ");
    strcpy(word[2],  "call      ");
    strcpy(word[3],  "const     ");
    strcpy(word[4],  "do        ");
    strcpy(word[5],  "else      ");
    strcpy(word[6],  "end       ");
    strcpy(word[7],  "exit      ");
    strcpy(word[8],  "false     ");
    strcpy(word[9],  "if        ");
    strcpy(word[10], "not       ");
    strcpy(word[11], "odd       ");
    strcpy(word[12], "or        ");
    strcpy(word[13], "procedure ");
    strcpy(word[14], "read      ");
    strcpy(word[15], "then      ");
    strcpy(word[16], "true      ");
    strcpy(word[17], "var       ");
    strcpy(word[18], "while     ");
    strcpy(word[19], "write     ");
    
    wsym[0]=andsym;
    wsym[1]=beginsym;
    wsym[2]=callsym;
    wsym[3]=constsym;
    wsym[4]=dosym;
    wsym[5]=elsesym;
    wsym[6]=endsym;
    wsym[7]=exitsym;
    wsym[8]=falsesym;
    wsym[9]=ifsym;
    wsym[10]=notsym;
    wsym[11]=oddsym;
    wsym[12]=orsym;
    wsym[13]=procsym;
    wsym[14]=readsym;
    wsym[15]=thensym;
    wsym[16]=truesym;
    wsym[17]=varsym;
    wsym[18]=whilesym;
    wsym[19]=writesym;

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
    statbegsys=beginsym|callsym|ifsym|whilesym|readsym|writesym;
    simpexpbegsys=plus|minus|orsym;
    termbegsys=times|slash|andsym;
    facbegsys=ident|number|lparen|oddsym|truesym|falsesym|notsym;

    err=0;                       // ���������
    cc=0;
    cx=0;                        // �м����ָ������
    ll=0;
    ch=' ';
    kk=al;
    lev=0;                       // �����㼶����
    tx=0;                        // ���ű�ָ������
    elx=0;                       // exitlist����
}
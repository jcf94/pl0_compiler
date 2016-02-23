/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Define_h
************************************************ */

#include <cstdio>

//---------------------------------
//-------------Define
//---------------------------------

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

#define stacksize   50000

//---------------------------------
//-------------Error
//---------------------------------

extern char* err_msg[33];        // ������Ϣ��
extern long err;                 // �������

void error(long);                // ���ش�����Ϣ

//---------------------------------
//-------------Lexer
//---------------------------------

extern long cc;                  // �м���
extern long ll;                  // �г���
extern FILE* infile;             // �����ļ�
extern long cx;                  // �������ָ��
extern char ch;                  // ���������һ���ַ�
extern char line[81];            // �л�����
extern char a[al+1];             // �ʷ������������ڴ�����ڱ��������Ĵ�
extern long kk;                  // ��ǰtoken�ĳ���
extern char id[al+1];            // ���������һ����ʶ��
extern char word[norw][al+1];    // �����ֱ�
extern unsigned long wsym[norw]; // ÿһ�������ֶ�Ӧ��symbol���ͱ�
extern unsigned long ssym[256];  // ÿһ�����Ŷ�Ӧ��symbol���ͱ�
extern unsigned long sym;        // ���������һ��token������
extern long num;                 // ���������һ������

void getch();                    // ��ȡ�����ַ�
void getsym();                   // ��ȡһ��token
void test(unsigned long, unsigned long, long);
                                 // ���Ե�ǰtoken�Ƿ�Ϸ�

//---------------------------------
//-------------Environment
//---------------------------------

extern long tx;                  // ���ű�ָ��
enum object
{
    constant, variable, proc
};
struct tabletype
{
    char name[al+1];
    enum object kind;
    long val;
    long level;
    long addr;
};
extern tabletype table[txmax+1]; // ���ű�
extern long dx;                  // ����ջָ��
extern long lev;                 // ��ǰ�㼶

void enter(enum object);         // ��¼���ű�
long position(char* id);         // ���ҷ��ű�

//---------------------------------
//-------------Generate
//---------------------------------

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

extern instruction code[cxmax+1];// �м����
extern char mnemonic[8][3+1];    // �м�������Ƿ���

void gen(enum fct, long, long);  // ���ɴ���
void listcode(long);             // �������

//---------------------------------
//-------------Parse
//---------------------------------

extern unsigned long declbegsys; // decl��ʼ���ż���
extern unsigned long statbegsys; // stmt��ʼ���ż���
extern unsigned long facbegsys;  // factor��ʼ���ż���

void block(unsigned long);
void constdeclaration();
void vardeclaration();
void statement(unsigned long);
void condition(unsigned long);
void expression(unsigned long);
void term(unsigned long);
void factor(unsigned long);

//---------------------------------
//-------------Interpretation
//---------------------------------

extern long s[stacksize];        // ����ջ

long base(long, long);
void interpret();
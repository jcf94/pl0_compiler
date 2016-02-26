/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Define_h
************************************************ */

#include <cstdio>

//---------------------------------
//-------------Define
//---------------------------------

#define norw        25             // no. of reserved words
#define txmax       100            // length of identifier table
#define nmax        14             // max. no. of digits in numbers
#define al          10             // length of identifiers
#define amax        2047           // maximum address
#define levmax      3              // maximum depth of block nesting
#define cxmax       2000           // size of code array
#define elsize      2000           // while�е�exit��ĳ���
#define etlsize     2000           // ���������е�enterlist��ĳ���
#define emsgsize    40             // ������Ϣ��ĳ���

#define nul         0x1            // ��
#define ident       0x2            // ��ʶ��
#define number      0x4            // ��ֵ
#define plus        0x8            // +
#define minus       0x10           // -
#define times       0x20           // *
#define slash       0x40           // /
#define oddsym      0x80           // �����ж�
#define eql         0x100          // =
#define neq         0x200          // <>
#define lss         0x400          // <
#define leq         0x800          // <=
#define gtr         0x1000         // >
#define geq         0x2000         // >=
#define lparen      0x4000         // (
#define rparen      0x8000         // )
#define comma       0x10000        // ,
#define semicolon   0x20000        // ;
#define period      0x40000        // .
#define becomes     0x80000        // :=
#define beginsym    0x100000       // �����֣�begin
#define endsym      0x200000       // �����֣�end
#define ifsym       0x400000       // �����֣�if
#define thensym     0x800000       // �����֣�then
#define whilesym    0x1000000      // �����֣�while
#define dosym       0x2000000      // �����֣�do
#define callsym     0x4000000      // �����֣�call
#define constsym    0x8000000      // �����֣�const
#define varsym      0x10000000     // �����֣�var
#define procsym     0x20000000     // �����֣�procedure
#define elsesym     0x40000000     // �����֣�else
#define exitsym     0x80000000     // �����֣�exit
#define readsym     0x100000000    // �����֣�read
#define writesym    0x200000000    // �����֣�write
#define falsesym    0x400000000    // �����֣�false
#define truesym     0x800000000    // �����֣�true
#define andsym      0x1000000000   // �����֣�and
#define orsym       0x2000000000   // �����֣�or
#define notsym      0x4000000000   // �����֣�not
#define divsym      0x8000000000   // �����֣�div
#define modsym      0x10000000000  // �����֣�mod
#define intsym      0x20000000000  // �����֣�integer
#define realsym     0x40000000000  // �����֣�real
#define boolsym     0x80000000000  // �����֣�boolean
#define colon       0x100000000000 // :

#define stacksize   50000

//---------------------------------
//-------------Error
//---------------------------------

extern char* err_msg[emsgsize];  // ������Ϣ��
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
extern unsigned long long wsym[norw]; // ÿһ�������ֶ�Ӧ��symbol���ͱ�
extern unsigned long long ssym[256];  // ÿһ�����Ŷ�Ӧ��symbol���ͱ�
extern unsigned long long sym;        // ���������һ��token������
extern long num;                 // ���������һ������

void getch();                    // ��ȡ�����ַ�
void getsym();                   // ��ȡһ��token
void test(unsigned long long, unsigned long long, long);
                                 // ���Ե�ǰtoken�Ƿ�Ϸ�

//---------------------------------
//-------------Environment
//---------------------------------

extern long tx;                  // ���ű�ָ��
enum object
{
    constant, variable, proc, integer, boolean, real
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
extern long enterlist[etlsize];  // ���������е�enterlist��
extern long etlx;                // enterlistָ��

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

extern unsigned long long declbegsys; // decl��ʼ���ż���
extern unsigned long long statbegsys; // stmt��ʼ���ż���
extern unsigned long long simpexpbegsys;// simpexp��ʼ���ż���
extern unsigned long long termbegsys; // term��ʼ���ż���
extern unsigned long long facbegsys;  // factor��ʼ���ż���
extern long exitlist[elsize];    // while�е�exit��ַ��
extern long elx;                 // exitlistָ��

void block(unsigned long long);
void constdeclaration();
void vardeclaration();
void statement(unsigned long long);
void expression(unsigned long long);
void simpexpression(unsigned long long);
void term(unsigned long long);
void factor(unsigned long long);

//---------------------------------
//-------------Interpretation
//---------------------------------

extern long s[stacksize];        // ����ջ

long base(long, long);
void interpret();

//---------------------------------
//-------------Initialization
//---------------------------------

void globalinit();
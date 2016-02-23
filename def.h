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

extern char* err_msg[33];        // 错误信息表
extern long err;                 // 错误计数

void error(long);                // 返回错误信息

//---------------------------------
//-------------Lexer
//---------------------------------

extern long cc;                  // 列计数
extern long ll;                  // 行长度
extern FILE* infile;             // 输入文件
extern long cx;                  // 代码分配指针
extern char ch;                  // 读到的最后一个字符
extern char line[81];            // 行缓冲区
extern char a[al+1];             // 词法分析器中用于存放正在被被分析的词
extern long kk;                  // 当前token的长度
extern char id[al+1];            // 读到的最后一个标识符
extern char word[norw][al+1];    // 保留字表
extern unsigned long wsym[norw]; // 每一个保留字对应的symbol类型表
extern unsigned long ssym[256];  // 每一个符号对应的symbol类型表
extern unsigned long sym;        // 读到的最后一个token的类型
extern long num;                 // 读到的最后一个数字

void getch();                    // 读取单个字符
void getsym();                   // 读取一个token
void test(unsigned long, unsigned long, long);
                                 // 测试当前token是否合法

//---------------------------------
//-------------Environment
//---------------------------------

extern long tx;                  // 符号表指针
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
extern tabletype table[txmax+1]; // 符号表
extern long dx;                  // 数据栈指针
extern long lev;                 // 当前层级

void enter(enum object);         // 登录符号表
long position(char* id);         // 查找符号表

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

extern instruction code[cxmax+1];// 中间代码
extern char mnemonic[8][3+1];    // 中间代码助记符表

void gen(enum fct, long, long);  // 生成代码
void listcode(long);             // 输出代码

//---------------------------------
//-------------Parse
//---------------------------------

extern unsigned long declbegsys; // decl开始符号集合
extern unsigned long statbegsys; // stmt开始符号集合
extern unsigned long facbegsys;  // factor开始符号集合

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

extern long s[stacksize];        // 数据栈

long base(long, long);
void interpret();
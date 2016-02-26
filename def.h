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
#define elsize      2000           // while中的exit表的长度
#define etlsize     2000           // 变量声明中的enterlist表的长度
#define emsgsize    40             // 错误信息表的长度

#define nul         0x1            // 空
#define ident       0x2            // 标识符
#define number      0x4            // 数值
#define plus        0x8            // +
#define minus       0x10           // -
#define times       0x20           // *
#define slash       0x40           // /
#define oddsym      0x80           // 奇数判断
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
#define beginsym    0x100000       // 保留字：begin
#define endsym      0x200000       // 保留字：end
#define ifsym       0x400000       // 保留字：if
#define thensym     0x800000       // 保留字：then
#define whilesym    0x1000000      // 保留字：while
#define dosym       0x2000000      // 保留字：do
#define callsym     0x4000000      // 保留字：call
#define constsym    0x8000000      // 保留字：const
#define varsym      0x10000000     // 保留字：var
#define procsym     0x20000000     // 保留字：procedure
#define elsesym     0x40000000     // 保留字：else
#define exitsym     0x80000000     // 保留字：exit
#define readsym     0x100000000    // 保留字：read
#define writesym    0x200000000    // 保留字：write
#define falsesym    0x400000000    // 保留字：false
#define truesym     0x800000000    // 保留字：true
#define andsym      0x1000000000   // 保留字：and
#define orsym       0x2000000000   // 保留字：or
#define notsym      0x4000000000   // 保留字：not
#define divsym      0x8000000000   // 保留字：div
#define modsym      0x10000000000  // 保留字：mod
#define intsym      0x20000000000  // 保留字：integer
#define realsym     0x40000000000  // 保留字：real
#define boolsym     0x80000000000  // 保留字：boolean
#define colon       0x100000000000 // :

#define stacksize   50000

//---------------------------------
//-------------Error
//---------------------------------

extern char* err_msg[emsgsize];  // 错误信息表
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
extern unsigned long long wsym[norw]; // 每一个保留字对应的symbol类型表
extern unsigned long long ssym[256];  // 每一个符号对应的symbol类型表
extern unsigned long long sym;        // 读到的最后一个token的类型
extern long num;                 // 读到的最后一个数字

void getch();                    // 读取单个字符
void getsym();                   // 读取一个token
void test(unsigned long long, unsigned long long, long);
                                 // 测试当前token是否合法

//---------------------------------
//-------------Environment
//---------------------------------

extern long tx;                  // 符号表指针
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
extern tabletype table[txmax+1]; // 符号表
extern long dx;                  // 数据栈指针
extern long lev;                 // 当前层级
extern long enterlist[etlsize];  // 变量声明中的enterlist表
extern long etlx;                // enterlist指针

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

extern unsigned long long declbegsys; // decl开始符号集合
extern unsigned long long statbegsys; // stmt开始符号集合
extern unsigned long long simpexpbegsys;// simpexp开始符号集合
extern unsigned long long termbegsys; // term开始符号集合
extern unsigned long long facbegsys;  // factor开始符号集合
extern long exitlist[elsize];    // while中的exit地址表
extern long elx;                 // exitlist指针

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

extern long s[stacksize];        // 数据栈

long base(long, long);
void interpret();

//---------------------------------
//-------------Initialization
//---------------------------------

void globalinit();
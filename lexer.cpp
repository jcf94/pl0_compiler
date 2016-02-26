/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Lexer
INST    : 词法分析
************************************************ */

#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include "def.h"

//---------------------------------
//-------------读取单个字符
//---------------------------------

void getch()
{
    if(cc == ll)
    {
        if(feof(infile))
        {
            printf("************************************\n");
            printf("      program incomplete\n");
            printf("************************************\n");
            exit(1);
        }
        
        ll = 0; cc = 0;
        
        printf("%5d ", cx);
        
        while((!feof(infile)) && ((ch=getc(infile))!='\n'))
        {
            printf("%c", ch);
            ll = ll + 1; 
            line[ll] = ch;
        }

        printf("\n");
        
        ll = ll + 1; 
        line[ll] = ' ';
    }

    cc = cc + 1; 
    ch = line[cc];
}

//---------------------------------
//-------------读取一个token
//---------------------------------

void getsym()
{
    long i, j, k;

    while(ch == ' ' || ch == '\t')
    {
        getch();
    }

    if (isalpha(ch))             // token首位是字母
    {
        k = 0;

        do
        {
            if(k < al)
            {
                a[k] = ch; k = k + 1;
            }

            getch();
        } while(isalpha(ch) || isdigit(ch));

        if(k >= kk)              // 行末补上空格，以便下面的对比
        {
            kk = k;
        }
        else
        {
            do
            {
                kk = kk-1; a[kk] = ' ';
            } while(k < kk);
        }

        strcpy(id, a); i = 0; j = norw - 1;

        do                       // 在保留字表中二分查找确定保留字
        {
            k = (i+j)/2;

            if(strcmp(id, word[k]) <= 0)
            {
                j = k - 1;
            }

            if(strcmp(id, word[k]) >=0)
            {
                i = k + 1;
            }
        } while(i <= j);
      
        if(i-1 > j)
        {
            sym = wsym[k];
        }
        else
        {
            sym = ident;
        }
    }
    else if (isdigit(ch))        // token首位是数字
    {
        k = 0; num = 0; sym = number;
        do
        {
            num = num * 10 + (ch - '0');
            k = k + 1;
            getch();
        } while(isdigit(ch));
        
        if(k > nmax)
        {
            error(31);
        }
    }
    else if (ch == ':')          // token首位是“:”
    {
        getch();
        
        if(ch == '=')
        {
            sym = becomes; getch();
        }
        else sym = colon;
    }
    else if (ch == '<')          // token首位是“<”
    {
        getch();
    
        if(ch == '=')
        {
            sym = leq; getch();
        }
        else if(ch == '>')
        {
            sym=neq; getch();
        }
        else
        {
            sym = lss;
        }
    }
    else if (ch == '>')          // token首位是“>”
    {
        getch();
        
        if(ch == '=')
        {
            sym=geq; getch();
        }
        else
        {
            sym=gtr;
        }
    }
    else if (ch == '/')          // token首位是“/”，处理注释
    {
        getch();

        if (ch == '/')
        {
            long l1=ll;
            getch();
            while (l1==ll) getch();

            getsym();
        } else if (ch == '*')
        {
            getch();
            bool doit=true;
            while(doit)
            {
                while(ch != '*') getch();
                getch();
                if (ch == '/')
                {
                    doit=false;
                    getch();
                }
            }

            getsym();
        } else sym = ssym[(unsigned char)'/'];
    }
    else
    {
        sym = ssym[(unsigned char)ch]; getch();
    }
}

//---------------------------------
//-------------token测试
//测试遇到的当前token是否是属于s1这个集合里的
//若不是，则报n号错误，并将词素集合加上s2
//继续后续token扫描
//---------------------------------

void test(unsigned long long s1, unsigned long long s2, long n)
{
    if (!(sym & s1))
    {
        error(n);
        s1 = s1 | s2;
        
        while(!(sym & s1))
        {
            getsym();
        }
    }
}
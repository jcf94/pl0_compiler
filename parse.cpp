/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Parse
INST    : 语法分析
************************************************ */

#include <cstdio>

#include "def.h"

void block(unsigned long long fsys)
{
    long tx0;       // initial table index
    long cx0;       // initial code index
    long tx1;       // save current table index before processing nested procedures
    long dx1;       // save data allocation index

    dx=3; tx0=tx; table[tx].addr=cx; gen(jmp,0,0);

    if(lev>levmax)
    {
        error(32);
    }

    do
    {
        if(sym==constsym)
        {
            getsym();

            do
            {
                constdeclaration();
                while(sym==comma)
                {
                    getsym(); constdeclaration();
                }

                if(sym==semicolon)
                {
                    getsym();
                }
                else
                {
                    error(5);
                }
            } while(sym==ident);
        }

        if(sym==varsym)
        {
            getsym();
            do
            {
                vardeclaration();
                while(sym==comma)
                {
                    getsym(); vardeclaration();
                }

                if(sym==semicolon)
                {
                    getsym();
                }
                else
                {
                    error(5);
                }
            } while(sym==ident);
        }

        while(sym==procsym)
        {
            getsym();
            if(sym==ident)
            {
                enter(proc); getsym();
            }
            else
            {
                error(4);
            }

            if(sym==semicolon)
            {
                getsym();
            }
            else
            {
                error(5);
            }

            lev=lev+1; tx1=tx; dx1=dx;
            block(fsys|semicolon);
            lev=lev-1; tx=tx1; dx=dx1;

            if(sym==semicolon)
            {
                getsym();
                test(statbegsys|ident|procsym,fsys,6);
            }
            else
            {
                error(5);
            }
        }

        test(statbegsys|ident,declbegsys,7);
    } while(sym&declbegsys);

    code[table[tx0].addr].a=cx;
    table[tx0].addr=cx;     // start addr of code
    cx0=cx; gen(Int,0,dx);
    statement(fsys|semicolon|endsym);
    gen(opr,0,0); // return
    test(fsys,0,8);
    //listcode(cx0);
}

void constdeclaration()
{
    if(sym == ident)
    {
        getsym();

        if(sym == eql || sym == becomes)
        {
            if(sym == becomes)
            {
                error(1);
            }

            getsym();

            if(sym == number)
            {
                enter(constant);
                getsym();
            }
            else
            {
                error(2);
            }
        }
        else
        {
            error(3);
        }
    }
    else
    {
        error(4);
    }
}

void vardeclaration()
{
    if(sym == ident)
    {
        enter(variable);
        getsym();
    }
    else
    {
        error(4);
    }
}

void statement(unsigned long long fsys)
{
    long i,cx1,cx2;

    switch(sym)
    {
        case ident:              // 以标识符开始，则为赋值语句
        {
            i=position(id);
            if(i==0)
            {
                error(11);
            }
            else if(table[i].kind!=variable)    // assignment to non-variable
            {
                error(12); i=0;
            }

            getsym();

            if(sym==becomes)
            {
                getsym();
            }
            else
            {
                error(13);
            }

            simpexpression(fsys);

            if(i!=0)
            {
                gen(sto,lev-table[i].level,table[i].addr);
            }
        } break;
        case callsym:            // 调用语句
        {
            getsym();
            if(sym!=ident)
            {
                error(14);
            }
            else
            {
                i=position(id);
                if(i==0)
                {
                    error(11);
                }
                else if(table[i].kind==proc)
                {
                    gen(cal,lev-table[i].level,table[i].addr);
                }
                else
                {
                    error(15);
                }

                getsym();
            }
        } break;
        case ifsym:              // if语句
        {
            getsym();
            expression(fsys|thensym|dosym);

            if(sym==thensym)
            {
                getsym();
            }
            else
            {
                error(16);
            }
            cx1=cx;              // 记录下跳转代码的位置，此时跳转地址是0
            gen(jpc,0,0);
            statement(fsys|elsesym);// 紧接着可能是else
            code[cx1].a=cx;      // 把跳转地址加上

            if (sym==elsesym)    // else子句
            {
                cx1=cx;          // 记录下跳转代码的位置
                gen(jmp,0,0);    // 如果有else，那么then后面部分执行完毕后要跳过这段
                getsym();
                statement(fsys);
                code[cx1].a=cx;
            }
        } break;
        case beginsym:           // begin语句
        {
            getsym();
            statement(fsys|semicolon|endsym);
            while(sym==semicolon||(sym&statbegsys))
            {
                if(sym==semicolon)
                {
                    getsym();
                }
                else
                {
                    error(10);
                }
                statement(fsys|semicolon|endsym);
            }
            if(sym==endsym)
            {
                getsym();
            }
            else
            {
                error(17);
            }
        } break;
        case whilesym:           // while语句
        {
            cx1=cx; getsym();
            expression(fsys|dosym);

            cx2=elx;                 // 记录下当前层开始的elx
            exitlist[elx]=cx;        // 将jpc的位置记录进exitlist
            elx++;
            //cx2=cx;
            gen(jpc,0,0);
            if(sym==dosym)
            {
                getsym();
            }
            else
            {
                error(18);
            }

            statement(fsys|exitsym);
            gen(jmp,0,cx1);

            //code[cx2].a=cx;
            while(elx>cx2)           // 将exitlist中记录下的跳转语句的地址补充完整
            {
                elx--;
                code[exitlist[elx]].a=cx;
            }
        } break;
        case elsesym:            // else语句
        {
            test(fsys,0,33);     // 具体的处理已经在if中完成了，这里只要判错即可
        } break;
        case exitsym:            // exit语句
        {
            test(fsys,0,34);         // exit语句报错处理 

            exitlist[elx]=cx;        // 将jpc的位置记录进exitlist
            elx++;
            gen(jmp,0,0);

            getsym();
        } break;
        case readsym:            // read语句
        {
            getsym();
            if (sym==lparen) getsym();
            else error(35);

            if (sym==ident)
            {
                i=position(id);
                if(i==0)
                {
                    error(11);
                }
                else if(table[i].kind!=variable)
                {
                    error(12); i=0;
                }

                gen(opr,0,15);
                if(i!=0)
                {
                    gen(sto,lev-table[i].level,table[i].addr);
                }

                getsym();
            } else error(36);

            while(sym==comma)
            {
                getsym();
                if (sym==ident)
                {
                    i=position(id);
                    if(i==0)
                    {
                        error(11);
                    }
                    else if(table[i].kind!=variable)
                    {
                        error(12); i=0;
                    }

                    gen(opr,0,15);
                    if(i!=0)
                    {
                        gen(sto,lev-table[i].level,table[i].addr);
                    }

                    getsym();
                } else error(36);
            }

            if (sym==rparen) getsym();
            else error(22);
        } break;
        case writesym:           // write语句
        {
            getsym();
            if (sym==lparen) getsym();
            else error(35);

            expression(fsys|comma|rparen);
            gen(opr,0,14);

            while(sym==comma)
            {
                getsym();
                expression(fsys|comma|rparen);
                gen(opr,0,14);
            }

            if (sym==rparen) getsym();
            else error(22);
        } break;
    }

    test(fsys,0,19);
}

void expression(unsigned long long fsys)
{
    unsigned long relop;

    simpexpression(fsys|eql|neq|lss|gtr|leq|geq);

    if(sym&(eql|neq|lss|gtr|leq|geq))
    {
        relop=sym; getsym();

        simpexpression(fsys);

        switch(relop)
        {
            case eql:
                gen(opr, 0, 8);
                break;

            case neq:
                gen(opr, 0, 9);
                break;

            case lss:
                gen(opr, 0, 10);
                break;

            case geq:
                gen(opr, 0, 11);
                break;

            case gtr:
                gen(opr, 0, 12);
                break;

            case leq:
                gen(opr, 0, 13);
                break;
        }
    }
}

void simpexpression(unsigned long long fsys)
{
    unsigned long long addop;

    if(sym==plus || sym==minus)  // 处理开头的正负号
    {
        addop=sym; getsym();

        term(fsys|simpexpbegsys);

        if(addop==minus)
        {
            gen(opr,0,1);
        }
    }
    else term(fsys|simpexpbegsys);

    while(sym & simpexpbegsys)
    {
        addop=sym; getsym();

        term(fsys|simpexpbegsys);

        switch(addop)
        {
            case plus:
                gen(opr,0,2);
                break;
            case minus:
                gen(opr,0,3);
                break;
            case orsym:
                gen(opr,0,18);
                break;
        }
    }
}

void term(unsigned long long fsys)
{
    unsigned long long mulop;

    factor(fsys|termbegsys);

    while(sym & termbegsys)
    {
        mulop = sym;
        getsym();

        factor(fsys|termbegsys);

        switch(mulop)
        {
            case times:
                gen(opr,0,4);
                break;
            case slash:
                gen(opr,0,5);
                break;
            case andsym:
                gen(opr,0,17);
                break;
            case divsym:
                gen(opr,0,19);
                break;
            case modsym:
                gen(opr,0,20);
                break;
        }
    }
}

void factor(unsigned long long fsys)
{
    long i;

    test(facbegsys, fsys, 24);

    while(sym & facbegsys)
    {
        switch(sym)
        {
            case ident:
            {
                i = position(id);

                if(i==0) error(11);
                else
                {
                    switch(table[i].kind)
                    {
                        case constant:
                            gen(lit, 0, table[i].val);
                            break;

                        case variable:
                            gen(lod, lev-table[i].level, table[i].addr);
                            break;

                        case proc:
                            error(21);
                            break;
                    }
                }

                getsym();
            } break;
            case number:
            {
                if(num>amax)
                {
                    error(31);
                    num=0;
                }

                gen(lit,0,num);
                getsym();
            } break;
            case lparen:
            {
                getsym();
                expression(rparen|fsys);

                if(sym==rparen) getsym();
                else error(22);
            } break;
            case oddsym:
            {
                getsym();

                if(sym == lparen) getsym();
                else error(35);

                simpexpression(fsys|rparen);
                gen(opr, 0, 6);

                if(sym == rparen) getsym();
                else error(22);
            } break;
            case falsesym:
            {
                gen(lit,0,0);
                getsym();
            } break;
            case truesym:
            {
                gen(lit,0,1);
                getsym();
            } break;
            case notsym:
            {
                getsym();

                factor(fsys);
                gen(opr,0,16);

            } break;
        }

        test(fsys,lparen,23);
    }
}

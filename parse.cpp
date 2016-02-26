/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Parse
INST    : �﷨����
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
        case ident:              // �Ա�ʶ����ʼ����Ϊ��ֵ���
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
        case callsym:            // �������
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
        case ifsym:              // if���
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
            cx1=cx;              // ��¼����ת�����λ�ã���ʱ��ת��ַ��0
            gen(jpc,0,0);
            statement(fsys|elsesym);// �����ſ�����else
            code[cx1].a=cx;      // ����ת��ַ����

            if (sym==elsesym)    // else�Ӿ�
            {
                cx1=cx;          // ��¼����ת�����λ��
                gen(jmp,0,0);    // �����else����ôthen���沿��ִ����Ϻ�Ҫ�������
                getsym();
                statement(fsys);
                code[cx1].a=cx;
            }
        } break;
        case beginsym:           // begin���
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
        case whilesym:           // while���
        {
            cx1=cx; getsym();
            expression(fsys|dosym);

            cx2=elx;                 // ��¼�µ�ǰ�㿪ʼ��elx
            exitlist[elx]=cx;        // ��jpc��λ�ü�¼��exitlist
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
            while(elx>cx2)           // ��exitlist�м�¼�µ���ת���ĵ�ַ��������
            {
                elx--;
                code[exitlist[elx]].a=cx;
            }
        } break;
        case elsesym:            // else���
        {
            test(fsys,0,33);     // ����Ĵ����Ѿ���if������ˣ�����ֻҪ�д���
        } break;
        case exitsym:            // exit���
        {
            test(fsys,0,34);         // exit��䱨���� 

            exitlist[elx]=cx;        // ��jpc��λ�ü�¼��exitlist
            elx++;
            gen(jmp,0,0);

            getsym();
        } break;
        case readsym:            // read���
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
        case writesym:           // write���
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

    if(sym==plus || sym==minus)  // ����ͷ��������
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

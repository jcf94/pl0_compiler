/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Parse
INST    : �﷨����
************************************************ */

#include <cstdio>

#include "def.h"

void block(unsigned long fsys)
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

void statement(unsigned long fsys)
{
    long i,cx1,cx2;

    if(sym==ident)               // �Ա�ʶ����ʼ����Ϊ��ֵ���
    {
        i=position(id);
        if(i==0)
        {
            error(11);
        }
        else if(table[i].kind!=variable)	// assignment to non-variable
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
    }
    else if(sym==callsym)        // �������
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
    }
    else if(sym==ifsym)          // if���
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
        cx1=cx;	gen(jpc,0,0);    // ��¼����ת�����λ�ã���ʱ��ת��ַ��0
        statement(fsys|elsesym); // �����ſ�����else
        code[cx1].a=cx;          // ����ת��ַ����

        if (sym==elsesym)        // else�Ӿ�
        {
            getsym();

            statement(fsys);
        }
    }
    else if(sym==beginsym)       // begin���
    {
        getsym(); statement(fsys|semicolon|endsym);
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
    }
    else if(sym==whilesym)
    {
        cx1=cx; getsym();
        expression(fsys|dosym);
        cx2=cx;	gen(jpc,0,0);
        if(sym==dosym)
        {
            getsym();
        }
        else
        {
            error(18);
        }

        statement(fsys); gen(jmp,0,cx1);

        code[cx2].a=cx;
    }
    else if(sym==elsesym)
    {
        test(fsys,0,33);
    }

    test(fsys,0,19);
}

void expression(unsigned long fsys)
{
    unsigned long relop;

    if(sym==oddsym)
    {
        getsym(); simpexpression(fsys);
        gen(opr, 0, 6);
    }
    else
    {
        simpexpression(fsys|eql|neq|lss|gtr|leq|geq);

        if(!(sym&(eql|neq|lss|gtr|leq|geq)))
        {
            error(20);
        }
        else
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
}

void simpexpression(unsigned long fsys)
{
    unsigned long addop;

    if(sym==plus || sym==minus)
    {
        addop=sym; getsym();

        term(fsys|plus|minus);

        if(addop==minus)
        {
            gen(opr,0,1);
        }
    }
    else
    {
        term(fsys|plus|minus);
    }

    while(sym==plus || sym==minus)
    {
        addop=sym; getsym();

        term(fsys|plus|minus);

        if(addop==plus)
        {
            gen(opr,0,2);
        }
        else
        {
            gen(opr,0,3);
        }
    }
}

void term(unsigned long fsys)
{
    unsigned long mulop;

    factor(fsys|times|slash);

    while(sym==times || sym==slash)
    {
        mulop = sym; getsym();

        factor(fsys|times|slash);

        if(mulop == times)
        {
            gen(opr,0,4);
        }
        else{
            gen(opr,0,5);
        }
    }
}

void factor(unsigned long fsys)
{
    long i;

    test(facbegsys, fsys, 24);

    while(sym & facbegsys)
    {
        if(sym == ident)
        {
            i = position(id);

            if(i==0)
            {
                error(11);
            }
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
        }
        else if(sym == number)
        {
            if(num>amax)
            {
                error(31); num=0;
            }

            gen(lit,0,num);
            getsym();
        }
        else if(sym == lparen)
        {
            getsym();
            simpexpression(rparen|fsys);

            if(sym==rparen)
            {
                getsym();
            }
            else
            {
                error(22);
            }
        }

        test(fsys,lparen,23);
    }
}

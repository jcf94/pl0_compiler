/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Generate
INST    : ´úÂëÉú³É
************************************************ */

#include <cstdio>

#include "def.h"

void gen(enum fct x, long y, long z)
{
    if(cx > cxmax)
    {
        printf("program too long\n");
        //exit(1);
    }
    
    code[cx].f = x; code[cx].l = y; code[cx].a = z;
    
    cx = cx + 1;
}

void listcode(long cx0)         // list code generated for this block
{
    long i;

    for(i=cx0; i<=cx-1; i++)
    {
        printf("%10d%5s%3d%5d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
    }
}

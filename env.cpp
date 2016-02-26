/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Environment
INST    : ·ûºÅ±í´¦Àí
************************************************ */

#include <cstring>
#include <cstdio>

#include "def.h"

void enter(enum object k)
{
    tx++;

    strcpy(table[tx].name, id);
    
    table[tx].kind = k;
    
    switch(k)
    {
        case constant:
            if(num > amax)
            {
                error(31);
                num = 0;
            }
            table[tx].val = num;
            break;

        case variable:
            table[tx].level = lev; table[tx].addr = dx; dx = dx + 1;
            break;

        case proc:
            table[tx].level = lev;
            break;
    }
}

long position(char* id)
{
    long i;

    strcpy(table[0].name, id);
 
    i=tx;

    while(strcmp(table[i].name, id) != 0)
    {
        i = i - 1;
    }

    return i;
}
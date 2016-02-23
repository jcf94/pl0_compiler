/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/O_Err
INST    : ¥ÌŒÛ¥¶¿Ì
************************************************ */

#include <cstdio>
#include "def.h"

void error(long n)
{
    long i;

    printf("Error=>");
    for (i = 1; i <= cc-1; i++)
    {
        printf(" ");
    }
    
    printf("|%s(%d)\n", err_msg[n], n);
    
    err++;
}

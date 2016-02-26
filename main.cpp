/* ***********************************************
MYID    : Chen Fan
LANG    : G++
PROG    : PL/0_Main
INST    : PL/0±àÒëÆ÷Ö÷³ÌÐò
************************************************ */

#include <cstdlib>

#include "def.h"

char infilename[80];

int main()
{
    long i;

    globalinit();

    printf("please input source program file name: ");
    //scanf("%s",infilename);
    printf("\n");
  
    //if((infile=fopen(infilename,"r"))==NULL)
    if((infile=fopen("test1.pl0","r"))==NULL)
    {
        printf("File %s can't be opened.\n", infilename);
        exit(1);
    }

    getsym();
    block(declbegsys|statbegsys|period);
    
    if(sym!=period)
    {
        error(9);
    }
    
    fclose(infile);

    if(err==0)
    {
        listcode(0);
        interpret();
    }
    else
    {
        printf("%3ld errors in PL/0 program\n",err);
    }

    return (0);
}

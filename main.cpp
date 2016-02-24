/* ***********************************************
MYID    : Chen Fan
LANG    : G++
PROG    : PL/0_Main
INST    : PL/0编译器主程序
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

    err=0;
    cc=0; cx=0; ll=0; ch=' '; kk=al; getsym();
    lev=0; tx=0;
    block(declbegsys|statbegsys|period);
    
    if(sym!=period)
    {
        error(9);
    }
    
    if(err==0)
    {
       // interpret();
        listcode(0);
    }
    else
    {
        printf("errors in PL/0 program\n");
    }
    
    fclose(infile);

    return (0);
}

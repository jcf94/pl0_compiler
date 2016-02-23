/* ***********************************************
MYID    : Chen Fan
LANG    : G++
PROG    : PL/0_Main
INST    : PL/0编译器主程序
************************************************ */

#include <cstdlib>
#include <cstring>

#include "def.h"

char infilename[80];

int main()
{
    long i;

    for(i=0; i<256; i++)
    {
        ssym[i]=nul;
    }
    
    strcpy(word[0],  "begin     ");
    strcpy(word[1],  "call      ");
    strcpy(word[2],  "const     ");
    strcpy(word[3],  "do        ");
    strcpy(word[4],  "end       ");
    strcpy(word[5],  "if        ");
    strcpy(word[6],  "odd       ");
    strcpy(word[7],  "procedure ");
    strcpy(word[8],  "then      ");
    strcpy(word[9],  "var       ");
    strcpy(word[10], "while     ");
    
    wsym[0]=beginsym;
    wsym[1]=callsym;
    wsym[2]=constsym;
    wsym[3]=dosym;
    wsym[4]=endsym;
    wsym[5]=ifsym;
    wsym[6]=oddsym;
    wsym[7]=procsym;
    wsym[8]=thensym;
    wsym[9]=varsym;
    wsym[10]=whilesym;
    ssym['+']=plus;
    ssym['-']=minus;
    ssym['*']=times;
    ssym['/']=slash;
    ssym['(']=lparen;
    ssym[')']=rparen;
    ssym['=']=eql;
    ssym[',']=comma;
    ssym['.']=period;
    ssym[';']=semicolon;
  
    strcpy(mnemonic[lit],"LIT");
    strcpy(mnemonic[opr],"OPR");
    strcpy(mnemonic[lod],"LOD");
    strcpy(mnemonic[sto],"STO");
    strcpy(mnemonic[cal],"CAL");
    strcpy(mnemonic[Int],"INT");
    strcpy(mnemonic[jmp],"JMP");
    strcpy(mnemonic[jpc],"JPC");
  
    declbegsys=constsym|varsym|procsym;
    statbegsys=beginsym|callsym|ifsym|whilesym;
    facbegsys=ident|number|lparen;

    printf("please input source program file name: ");
    scanf("%s",infilename);
    printf("\n");
  
    if((infile=fopen(infilename,"r"))==NULL)
    //if((infile=fopen("test1.pl0","r"))==NULL)
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

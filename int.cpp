/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: PL/0_Interpretation
INST    : 中间代码解释器
************************************************ */

#include "def.h"

long base(long b, long l)
{
    long b1;

    b1=b;

    while (l>0)         // find base l levels down
    {
        b1=s[b1]; l=l-1;
    }

    return b1;
}

void interpret()
{
    long p,b,t;		// program-, base-, topstack-registers
    instruction i;	// instruction register

    printf("start PL/0\n");
    t=0; b=1; p=0;
    s[1]=0; s[2]=0; s[3]=0;
    
    do
    {
        i=code[p]; p=p+1;
    
        switch(i.f)
        {
            case lit:
                t=t+1; s[t]=i.a;
                break;
        
            case opr:
                switch(i.a) 	 // 运算指令
                {
                    case 0:	     // 过程调用返回
                        t=b-1; p=s[t+3]; b=s[t+2];
                        break;
                 
                    case 1:      // -
                        s[t]=-s[t];
                        break;
                
                    case 2:      // +
                        t=t-1; s[t]=s[t]+s[t+1];
                        break;
                 
                    case 3:      // -
                        t=t-1; s[t]=s[t]-s[t+1];
                        break;
                 
                    case 4:      // *
                        t=t-1; s[t]=s[t]*s[t+1];
                        break;
                
                    case 5:      // /
                        t=t-1; s[t]=s[t]/s[t+1];
                        break;
                 
                    case 6:      // odd
                        s[t]=s[t]%2;
                        break;
                 
                    case 8:      // ==
                        t=t-1; s[t]=(s[t]==s[t+1]);
                        break;
                 
                    case 9:      // !=
                        t=t-1; s[t]=(s[t]!=s[t+1]);
                        break;
                 
                    case 10:     // <
                        t=t-1; s[t]=(s[t]<s[t+1]);
                        break;
                 
                    case 11:     // >=
                        t=t-1; s[t]=(s[t]>=s[t+1]);
                        break;
                 
                    case 12:     // >
                        t=t-1; s[t]=(s[t]>s[t+1]);
                        break;
                 
                    case 13:     // <=
                        t=t-1; s[t]=(s[t]<=s[t+1]);
                        break;

                    case 14:     // 屏幕输出
                        printf("%d\n", s[t]);
                        break;

                    case 15:     // 屏幕输入
                        t=t+1; scanf("%d",&s[t]);
                        break;

                    case 16:     // not
                        if (s[t]) s[t]=0; else s[t]=1;
                        break;

                    case 17:     // and
                        t=t-1; s[t]=s[t]&s[t+1];
                        break;

                    case 18:     // or
                        t=t-1; s[t]=s[t]|s[t+1];
                        break;
                }
                break;
            
            case lod:
                t=t+1; s[t]=s[base(b,i.l)+i.a];
                break;
            
            case sto:
                s[base(b,i.l)+i.a]=s[t];
                //printf("%10d\n", s[t]);
                t=t-1;
                break;
            
            case cal:		// generate new block mark
                s[t+1]=base(b,i.l); s[t+2]=b; s[t+3]=p;
                b=t+1; p=i.a;
                break;
            
            case Int:
                t=t+i.a;
                break;
            
            case jmp:
                p=i.a;
                break;
            
            case jpc:
                if(s[t]==0)
                {
                    p=i.a;
                }
                t=t-1;
        }
    } while(p!=0);
    printf("\nend PL/0\n");
}
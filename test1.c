#include <stdio.h>
#include <stdlib.h>

void def(void){
#define PRINT
}

int main(){
    int a, b, c = 2, d = 1;
    int i,j;
    long ln,ln1;
    char c1[10];
    char s[3]={'0','0','\0'};
    char s1[]="ff";
    char f[2];

    __asm__ __volatile__ (
         "movl %2, %%eax\n"
         "movl %%eax, %0\n"
         "movl %%eax, %1"
        : "=r" (a), "=r"(b)
        : "r" (c)
        :"memory"
    );
    //printf("a:%d b:%d c:%d\n",a, b, c);
    i = d ?: -1;
    j = (d == 0) ?: c;
    printf("i:%d j:%d\n",i, j);
    printf("short int:%zu int:%zu long:%zu bytes,long long:%zu bytes\n",sizeof(short int),sizeof(int),sizeof(long int),sizeof(long long int));
    for(int e = 0; c1[e] != '\0'; e++){
        c1[e] = '1';
    printf("c1[%d]:%c \n",e,c1[e]);
    }

    f[0] = strtol(&s[0],0,16);
    ln = strtol(&s[0],0,16);
    //f[1] = strtol(&s1[0],0,16);
    //ln1 = strtol(&s1[0],0,16);
    #ifdef PRINT
    printf("f:0x%x ln :%ld f[1]:0x%x\n",*f,ln,f[1]);
    #endif
    return 0;
}

#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/stat.h>   // stat
#include <stdbool.h> 

main()
{


FILE *f=fopen("test.yaml","wb");

int i,j,s,c;
int h[3]={0,0,0};

for(i=0;i<25;i++)
{
        s=rand()%3;
        h[s]++;
        fprintf(f,"name:   enc_s%02d_n%02d\n", s,h[s]);
        fprintf(f,"   -slevel: %d\n", s);  
        fprintf(f,"   -index: %d\n",  h[s]);    
        c=1000+rand()%1000;          
        fprintf(f,"   -cycles: %f\n", (float)c);     
        fprintf(f,"   -energy: %f\n", 0.1 + (float)(rand()&0xffff)/65536.0 ); 

        fprintf(f,"   -function: encryption\n");   
        fprintf(f,"   -type: software\n\n");   




}}

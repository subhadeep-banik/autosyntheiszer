#include <stdio.h>
#include <string.h>
#include <stdint.h>

 
#include "aes128.h"



int iterations = 50000;

static void phex(uint8_t* str);
 


void main(void)
{
    int exit;
    unsigned char Key[16]={0x84 , 0x81 , 0x85 , 0xdf , 0xa9 , 0x51 , 0xf1 , 0x1e , 0x13 , 0x97 , 0x24 , 0x8a , 0x6a , 0x69 , 0x8b , 0x17};
    unsigned char PT[16]= {0xad , 0x40 , 0xa8 , 0x96 , 0xb1 , 0xc7 , 0xea , 0xa0 , 0x52 , 0xb1 , 0xa7 , 0x0b , 0xd6 , 0x45 , 0xdb , 0x66};
    unsigned char CT[16];
    enc_s01_n01(PT, Key, CT);
    phex((uint8_t*) CT);
}


 
static void phex(uint8_t* str)
{

 
    uint8_t len = 16;
 

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}
 


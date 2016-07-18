#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i;
    for(i=0;i<4;++i) 
        printf("%i ", (((i&0x01)^0x01)==0)); 
    printf("\n"); 
    return 0;
}

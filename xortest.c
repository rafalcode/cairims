#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
   /* declarations */
   int i;
   for(i=0;i<4;++i) 
       printf("%i ",(i%2 +i/2)%2);
   printf("\n");

   printf("exclusive OR on the two LSBs of a byte:\n");
   for(i=0;i<4;++i)
       printf("%i ",((i>>1)^(i & 0x01)));
   printf("\n"); 

   printf("ANTI-exclusive OR on the two LSBs of a byte:\n");
   for(i=0;i<4;++i)
       printf("%i ",(((i>>1)^(i & 0x01))==0));
   printf("\n"); 

   for(i=0;i<4;++i) 
       printf("%i ",((i>>1)==0));
   printf("\n"); 

   return 0;
}

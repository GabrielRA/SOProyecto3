#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4


void gen(char *dst)
{
    int i, n;  

    srand(time(NULL));               /* init seed */
    if ((dst = malloc(sizeof(char) * (4 +1))) == NULL)   /* caller will need to free this */
        return;
    for (i = 0; i < 4; i++ )
        printf("Hola");
        //if ((n = rand()) < 'A' && n > 'Z')
            dst[i] = n;
       
    dst[4 - 1] = 0;                   /* null terminate the string */
}


char *gen_string(int length) 
{    
    time_t t;
    unsigned int key = 0, n;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-#87###";
    size_t stringLen = 69;        
    char *randomString;
    

    randomString = malloc(sizeof(char) * (length +1));

    if (!randomString) return (char*)0;
    
    for ( n = 0; n < length; n++) 
    {            
        key = rand() % stringLen;  
        //printf("%d y el caracter es: %c\n", key,string[key] );
        randomString[n] = string[key];
    }

    randomString[length] = '\0';

    return randomString;
}


int main()
{
    srand(time(NULL));
   char *string = gen_string(4);
    printf("Fin del primer for \n");
   char *string1 = gen_string(4);
   
   printf("El string es %s\n", string) ;
   printf("El string1 es %s\n", string1) ;
   free(string);
   free(string1);
   return(0);
}



/*
int main()
{
   int i, n;
   time_t t;
   
   n = 5;
   
  
   srand((unsigned) time(&t));

  
   for( i = 0 ; i < n ; i++ ) 
   {
      printf("%d\n", rand() % 50);
   }
   printf("Fin del primer for \n");
   for( i = 0 ; i < n ; i++ ) 
   {
      printf("%d\n", rand() % 50);
   }
   
   return(0);
}*/
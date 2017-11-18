#include <stdio.h>
#include <string.h>

void cmp(char *anterior[], char *actual[], int n, int m)
{
    int k_mayor, i, np = 0 , mp = 0 ;
    if (n == m) k_mayor = n;
    else if (n < m) k_mayor = n;
    else  k_mayor = m;
    
    printf("Se itera sobre: %i\n", k_mayor);
    for (i = 0; i < k_mayor;)
    {
        int ret = strcmp(anterior[np], actual[mp]);
        printf("np = %i , mp = %i, i = %i ,ret: (%s %s %i)\n", np, mp,i, anterior[np], actual[mp], ret);
        if (ret < 0)
        {
            printf("Se eliminò %s --> ", anterior[np]);
            np ++;
            i ++;
        }
        else if (ret > 0)
        {
            printf("Se agregò %s --> ", actual[mp]);
            mp ++;
            i ++;
        }
        else
        {
            np++; 
            mp++;
            i ++;
        }
    }
    printf("np = %i\n", np);
    printf("mp = %i\n", mp);
    printf("i = %i\n", i);
}


int main(void)
{
   
   
    char *anterior[] = {"a","b", "c", "d", "e", "f", "g", "h", "i"};
    char *actual[] = {"a", "d", "e", "f", "g", "h", "i", "j", "k"}; 
    
    

/*
    char *anterior[] = {"a", "d", "e", "f", "g", "h", "i", "j", "k"}; 
    char *actual[] = {"a","b", "c", "d", "e", "f", "g", "h", "i"};
*/


/*
    char *anterior[] = {"a"};
    char *actual[] = {"b","c", "d"};
 */
    
 /*   
    char *anterior[] = {"a","b", "c", "d", "e", "f", "g"};
    char *actual[] = {"c", "d"};
*/

/*
    char *anterior[] = {"a","b", "c", "d", "e", "f", "g"};
    char *actual[] = {"a", "b", "d", "f", "g"};
*/

/*
    char *anterior[] = {"a","b", "d", "e", "f", "g"};
    char *actual[] = {"a", "b","c", "d", "f", "g", "h"};
  */ 

/*
    char *anterior[] = {"b", "xx"};
    char *actual[] = {"a","c", "d", "f", "g", "h"};
  */ 
  
  /*
    char *anterior[] = {"l", "m"};
    char *actual[] = {"a","c", "d", "j", "l", "m"};
  */
    int n = sizeof anterior / sizeof anterior[0];
    int m = sizeof actual / sizeof actual[0];
    
    cmp(anterior, actual, n, m);
}
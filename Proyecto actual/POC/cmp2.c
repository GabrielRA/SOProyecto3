#include <stdio.h>
#include <string.h>

#define LEN(x) sizeof(x)/sizeof(x[0])

int in_set( char * x[], int len,  char *match)
{
/*
	int i;
	for (i = 0; i < len; i++)
	{
		if (x[i] && !strcmp(x[i], match)) return 1;
	}
	return 0;
*/
    int i = 0, j = len - 1, ret ;
    while (i <= j)
    {
        int k = (i + j) / 2;
        ret = strcmp(x[k], match);
        
        if (ret > 0)
        {
            j = k - 1;
        } else if (ret < 0)
        {
            i = k + 1;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
 
void diff( char * x[], int lenx,  char * y[], int leny)
{
	int i;
	for (i = 0; i < lenx; i++)
	{
		if (x[i] && !in_set(y, leny, x[i])) printf("  %s\n", x[i]);
	}
}


int main(void)
{
  /*  
    char *anterior[] = {"a","b", "c", "d", "e", "f", "g", "h", "i"};
    char *actual[] = {"a", "d", "e", "f", "g", "h", "i", "j", "k"}; 
*/
/*
    char *anterior[] = {"a"};
    char *actual[] = {"b","c", "d"};
*/
/*
    char *anterior[] = {"b", "xx"};
    char *actual[] = {"a","c", "d", "f", "g", "h"};
 */
  
  /* 
    char *anterior[] = {"l", "m"};
    char *actual[] = {"a","c", "d", "j", "l", "m"};
  */
  
    char *anterior[] = {"lindo.png", "mariposa.txt"};
    char *actual[] = {"arbol.docx","casa.bin", "duende", "j.png", "mariposa."};
    
    printf("Eliminados\n");
    diff(anterior, LEN(anterior), actual, LEN(actual));
    printf("Agregados\n");
    diff(actual, LEN(actual), anterior, LEN(anterior));
    
    return 0;
}
#include <stdio.h>
#include <string.h>

int binary_search(char *a[], int n, char* x)
{
    int i = 0, j = n - 1 ;
    int ret ;
    
    while (i <= j)
    {
        int k = (i + j) / 2;
        
        ret = strcmp(a[k], x);
        
        if(ret > 0) // a[k] is less than x
        {
            j = k - 1;
        }
        else if (ret < 0) // x is less than a[k]
        {
            i = k + 1;
        }
        else
        {
            return k;
        }
    }
    return -1;
}

int main(void)
{
    
    char *a[] = {"array.c","array.h","hola.bin", "main.c", "main1.c", "test"};
    int n = sizeof a / sizeof a[0];
    printf("El tamaño es %i\n", n);
    
    printf("%s\n", a[0]);
    
    int i = binary_search(a, n, "main1.c");
    printf("%s is at index %d\n", "hola", i);
  
    return 0;
}
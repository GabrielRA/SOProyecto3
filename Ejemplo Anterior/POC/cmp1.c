#include <stdio.h>
#include <malloc.h>
#include <string.h>


int* cmp(int* A, int A_length, int* B, int B_length, int* result_length)
{
    int i,j,k;
    int* result = (int*) malloc( sizeof(int) * A_length );
    if(result == NULL)
    {
        *result_length = -1;
        return NULL;
    }
    for(i = j = k = 0; i < A_length && j < B_length ; )
    {
        if(A[i] == B[j]) ++i, ++j;                  /* cmp this element - to enable multiple exclusion drop '++j'  4,4,5,6 /4 --> 5,6 */
        if(A[i] < B[j]) result[ k++ ] = A [ i++ ];
        if(A[i] > B[j]) ++j;                        /* Ignorar los nùmeros menores */
    }
    while(i < A_length) result[k++] = A[i++];

    if( k < A_length)
    {
        int* tmp = (int*) realloc( result , sizeof(int) * k );
        if(tmp == NULL)
        {
            /* Error de pedir memoria para el resultado */
        }else
        {
            result = tmp;
        }
    }
    *result_length = k;
    return result;
}


int main()
{
    /*
    int a[6] = {1,2,3,4,5,6};
    int b[3] = {2,4,5};
    */
    
    /*
    int a[6] = {1,2,3,4,5,6};
    int b[3] = {1,2,5};
    */
    /*
    int a[6] = {1,2,3,4,5,6};
    int b[9] = {1,2,3,4,5,6,7,8,9};
    */
    
    int result_length;
    int i;
    //int *c = cmp(a,6,b,3,&result_length);
    //int *c = cmp(b,9,a,6,&result_length);
    
    
    int *c = cmp(a,6,b,9,&result_length);
    printf("Cantidad en el resultado : %i \n",result_length);
    for(i = 0; i < result_length; ++i) printf("%i ",c[i]);
    
    free(c);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int *array;
    size_t used;
    size_t size;
    
} Array;

void initArray(Array *a, size_t initSize)
{
    a->array = (int *)malloc(initSize * sizeof(int));
    a->used = 0;
    a->size = initSize;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void insertArray(Array *a, int element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (int *)realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

int main(void)
{
    Array a;
    int i;
    
    initArray(&a, 3);
    
    insertArray(&a, 1);
    insertArray(&a, 2);
    insertArray(&a, 3);
    insertArray(&a, 4);
      
   printf("%i\n", (int)a.used);
   printf("%i\n", a.array[3]);
    freeArray(&a);
}
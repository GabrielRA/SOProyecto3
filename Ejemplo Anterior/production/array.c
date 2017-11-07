
#include "array.h"

void initArray(Array *a, size_t initSize)
{
    a->array = (file_data *)malloc(initSize * sizeof(file_data));
    a->used = 0;
    a->size = initSize;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void insertArray(Array *a, file_data element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (file_data *)realloc(a->array, a->size * sizeof(file_data));
    }
    a->array[a->used++] = element;
}

int findArray(Array *a, int n, char* file_name)
{
    int i = 0, j = n - 1 ;
    int ret;
    while (i <= j)
    {
        int k = (i + j) / 2;
        ret = strcmp(a -> array[k].name, file_name);
        
        if (ret > 0)
        {
            j = k - 1;
        } else if (ret < 0)
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


void saveToFile(char *filename, Array *a)
{
    FILE *ofp = fopen(filename, "wb");
    if (ofp != 0)
    {
        if (fwrite(a->array, a->size * sizeof(file_data), 1, ofp) != 1)
        {
            fprintf(stderr, "Failed to write to %s\n", filename);
            exit(1);
        }
        fclose(ofp);
    }
}

void readFromFile(char *filename, Array * a)
{
    FILE *ifp = fopen(filename, "rb");
    if (ifp != 0)
    {
        if (fread(a->array, a->size * sizeof(file_data), 1, ifp) != 1)
        {
            fprintf(stderr, "Failed to read from %s\n", filename);
            exit(1);
        }
        fclose(ifp);
    }
}

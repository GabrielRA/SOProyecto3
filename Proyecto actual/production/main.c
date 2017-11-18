
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

// ESTRUCTURAS =================================================================

typedef struct
{
    char name[15];
    int age;
    int salary;
    char address[300];
} employee;


typedef struct 
{
    employee *array;
    size_t used;
    size_t size;
} Array;

//  FUNCIONES ==================================================================
void initArray(Array *a, size_t initSize)
{
    a->array = (employee *)malloc(initSize * sizeof(employee));
    a->used = 0;
    a->size = initSize;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void insertArray(Array *a, employee element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (employee *)realloc(a->array, a->size * sizeof(employee));
    }
    a->array[a->used++] = element;
}



void saveToFile(char *filename, Array *a)
{
    FILE *ofp = fopen(filename, "wb");
    if (ofp != 0)
    {
        if (fwrite(a->array, a->size * sizeof(employee), 1, ofp) != 1)
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
        if (fread(a->array, a->size * sizeof(employee), 1, ifp) != 1)
        {
            fprintf(stderr, "Failed to read from %s\n", filename);
            exit(1);
        }
        fclose(ifp);
    }
}




//  ENTRADA DEL PROGRAMA =======================================================
int main(void)
{
    
    Array a;
    
    initArray(&a, 2);
    
    //printf("%i\n", a.size * sizeof(employee));
    
    employee e;
    strcpy( e.name, "Mario");
    strcpy( e.address, "El alto de la bella casa de los últimos días ");
    e.age = 22;
    e.salary = 30000;
    insertArray(&a, e);
    
    
    
    //strcpy( e.name, "Jose");
    insertArray(&a, e);
    insertArray(&a, e);
    insertArray(&a, e);
    insertArray(&a, e);
    insertArray(&a, e);
    
    
    
    //printf("%s\n", a.array[0].name);
    //printf("%s\n", a.array[1].name);

    saveToFile("hola.bin", &a);


    printf("Tamaño %i\n", (int)a.size);
    printf("Cuantos hay %i\n", (int)a.used);
    

    freeArray(&a);
    
    initArray(&a, 8);
    readFromFile("hola.bin", &a);
    
    
    
    a.used = 6;
    printf("%s\n", a.array[5].address);
    printf("%i\n", (int)a.used);
    
    
     insertArray(&a, e);
    printf("%s\n", a.array[6].name);
    
     saveToFile("hola.bin", &a);
    
    freeArray(&a);
    


   
    return 0;
}
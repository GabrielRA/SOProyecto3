#ifndef ARRAY_H_
#define ARRAY_H_


#include <stdlib.h>
#include <stdio.h>

//  ESTRUCTURAS ================================================================

typedef struct
{
    char name[100];
    int size;
    struct date
    {
        int sec;
        int min;
        int hour;
        int date;
        int month;
        int year; 
    } modification_time_data;
    time_t modification_time;

} file_data;


typedef struct
{
    file_data *array;
    size_t used;
    size_t size;
} Array;


//  FUNCIONES ==================================================================

void initArray(Array *a, size_t initSize);
void freeArray(Array *a);
void insertArray(Array *a, file_data element);


void saveToFile(char *filename, Array *a);
void readFromFile(char *filename, Array * a);



#endif  /* ARRAY_H_ */

#include <time.h>
#include <langinfo.h>

#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "structs.h"


void scanFilesFromDirectory(Array *files, struct dirent **namelist, int n, char *directory) ;
void writeFileNumber(char * filename, int n) ;
int readFileCount(char *filename) ;


/**
 * Muestra los archivos del directorio, estos archivos están en una lista previa.
 * @Param: files : punterio al arreglo con los archivos
 * @Param: n : cantidad de archivos por imprimir.
 **/
void showFiles(Array *files, int n)
{
    int i;
    struct tm *tm;
    char datestring[256];
    for (i = 0; i < n; i++)
    {
        tm = localtime(&files->array[i].modification_time);
        strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
        printf(" ARCHIVO: %s , TAMAÑO: %i , FECHA DE MODIFICACIÓN : %s |||\n", files->array[i].name, files->array[i].size, datestring);
    }

}



int main(void)
{
    Array files, current_files, deleted_files, added_files, modified_files;
    
    initArray(&current_files, 1);
    initArray(&deleted_files, 1);
    initArray(&added_files, 1);
    initArray(&modified_files, 1);
    
    
    struct dirent **namelist;
    int n;
    n = scandir(".", &namelist, &filter, alphasort);
    initArray(&files, n);
    scanFilesFromDirectory(&files, namelist, n, "./");
    saveToFile(".meta/files_data.bin", &files);
    writeFileNumber(".meta/count.bin", n); 
    
    showFiles(&files, n);
    


   
    freeArray(&current_files);
    freeArray(&deleted_files);
    freeArray(&added_files);
    freeArray(&modified_files);
    
    freeArray(&files);
    
    return 0 ;

}
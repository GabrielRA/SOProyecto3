
#include <time.h>
#include <langinfo.h>

#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


#include "array.h"

int filter(const struct dirent * dir)
{
    const char *s = dir -> d_name;
    if (s[0] == '.') return 0;
    return 1;
}
void writeFileNumber(char * filename, int n)
{
    FILE *in = fopen(filename, "w");
    fprintf(in, "%i\n", n); 
    fclose(in);
}
int readFileCount(char *filename)
{
    int n;
    FILE *in = fopen(filename, "r");
    fscanf(in, "%i", &n); 
    fclose(in);
    return n;
}
void scanFilesFromDirectory(Array *files, struct dirent **namelist, int n)
{
    struct stat statBuffer;
    struct tm *tm;
    int i = 0;
    
    while (i < n)
    {
        if (stat(namelist[i] -> d_name, &statBuffer) == -1) continue;
        
        file_data file;
        strcpy( file.name, namelist[i] -> d_name);
        tm = localtime(&statBuffer.st_mtime);
        file.modification_time_data.sec = tm -> tm_sec;
        file.modification_time_data.min = tm -> tm_min;
        file.modification_time_data.hour = tm -> tm_hour;
        file.modification_time_data.date = tm -> tm_mday;
        file.modification_time_data.month = tm -> tm_mon;
        file.modification_time_data.year = tm -> tm_year;
        file.modification_time = statBuffer.st_mtime;
        file.size = statBuffer.st_size;
        insertArray(files, file);
        
        free(namelist[i]);
        
        i ++;
    }
    free(namelist);
    printf("%i\n", n);
}
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
    
    initArray(&deleted_files, 1);
    initArray(&added_files, 1);
    initArray(&modified_files, 1);
    
    
 /*
    struct dirent **namelist;
    int n;
    n = scandir(".", &namelist, &filter, alphasort);
    initArray(&files, n);
    scanFilesFromDirectory(&files, namelist, n);
    saveToFile(".meta/files_data.bin", &files);
    writeFileNumber(".meta/count.bin", n); 
    
    showFiles(&files, n);
*/

    //  Listado anterior  
    int n = readFileCount(".meta/count.bin");
    initArray(&files, n);
    readFromFile(".meta/files_data.bin", &files);
    
    
    struct dirent **namelist;
    int np;
    np = scandir(".", &namelist, &filter, alphasort);
    initArray(&current_files, np);
    scanFilesFromDirectory(&current_files, namelist, np);
    
    printf("Los archivos que estaban antes ======================================\n");
    showFiles(&files, n);
    printf(" Actualmente hay %i archivos ========================================\n", np);
    showFiles(&current_files, np);
    
    
    printf(" Y la diferencia ========================================\n");
    
    diff(&files, n, &current_files, np, &deleted_files);
    printf("\t\tELIMINADOS %i\n", (int)deleted_files.used);
    showFiles(&deleted_files, (int)deleted_files.used);
    
    diff(&current_files, np, &files, n, &added_files);
    printf("\t\tAGREGADOS %i\n", (int)added_files.used);
    showFiles(&added_files, (int)added_files.used);
    
    diffModified(&files, n, &current_files, np, &modified_files);
    printf("EN COMÚN modificados (%i) ============================================\n", (int)modified_files.used);
    showFiles(&modified_files, (int)modified_files.used);
   
   
   
    freeArray(&current_files);
    freeArray(&files);
    freeArray(&deleted_files);
    freeArray(&added_files);
    freeArray(&modified_files);
    
   
   
}
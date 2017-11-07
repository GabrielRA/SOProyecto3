
#include "structs.h"

#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

int in_set( Array *a, int len,  char *match);
void diff(Array *x, int lenx, Array *y, int leny, Array *res);
void diffModified(Array *x, int lenx, Array *y, int leny, Array *res);
void compare(char *directory, Array *added_files, Array *modified_files, Array *deleted_files) ;

//  FUNCIONES PARA LA COMPARACIÓN DE LISTADOS DE DIRECTORIO ====================
// =============================================================================

/**
 *  Determina si un elemento se encuentra en un listado de directorio
 **/
int in_set( Array *a, int len,  char *match)
{
    int i = 0, j = len - 1, ret ;
    while (i <= j)
    {
        int k = (i + j) / 2;
        ret = strcmp(a -> array[k].name, match);
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

/**
 * Realiza la diferencia entre el listado de dos directorios.
 * Se lleva acabo la operación A - B sobre dos arreglos de archivos
 * @param: x : Array -> arreglo A
 * @param: lenx : tamaño del arreglo x
 * @param: y : Array -> arreglo B
 * @param: leny : tamaño del arreglo y
 * @param: res : Array -> arreglo para almacenar el resultado 
 **/
void diff(Array *x, int lenx, Array *y, int leny, Array *res)
{
    int i;
	for (i = 0; i < lenx; i++)
	{
		if (x->array[i].name && !in_set(y, leny, x->array[i].name)) insertArray(res, x->array[i] );//printf("  archivo -> %s\n", x->array[i].name);
	}
}

/**
 * Realiza la búsqueda de los archivos que ha sido modificados.
 * Compara el listado actual del directorio y el listado anterior
 **/
 void diffModified(Array *x, int lenx, Array *y, int leny, Array *res)
 {
    int i = 0, j = 0, ret ;
    
    while (i < lenx && j < leny)
    {
        ret = strcmp(x -> array[i].name, y -> array[j].name);
        
        if (ret < 0) i ++;
        else if (ret > 0) j ++;
        else
        {
            time_t t1 = x -> array[i].modification_time, t2 = y -> array[j].modification_time ;
            if ( t1 < t2) // t1 es más antiguo que t2;
            {
                insertArray(res, y -> array[j++] );//printf("%s\n", y -> array[j++].name);
            }
            else if ( t1 > t2 ) // t1 es más nuevo que t2
            {
                
            }
            i ++;  
        }
        
    }
 }


//  FUNCIONES UTILITARIAS PARA MANEJOR DE METADATOS Y ESCANEO DE DIRECTORIO ====
// =============================================================================


/**
 * Esta función escanea la lista de archivos obtenidas en scandir y las almacena
 * en la estructura <<file_data>>
 * @Param: files : puntero a un arreglo de archivos.
 * @Param: namelist : puntero al resultado de la función scandir.
 * @Param: n : cantidad de archivos por leer.
 * @Return: None
 **/
void scanFilesFromDirectory(Array *files, struct dirent **namelist, int n, char *directory)
{
    struct stat statBuffer;
    int i = 0;
    
    while (i < n)
    {
        char src[50];
        strcpy(src,  directory);
        strcat(src, namelist[i] -> d_name);
        if (stat(src, &statBuffer) != -1) 
        {
            file_data file;
            strcpy( file.name, namelist[i] -> d_name);
            strcpy( file.path, src) ;
            file.modification_time = statBuffer.st_mtime;
            file.size = statBuffer.st_size;
            insertArray(files, file);
        }
        
        free(namelist[i]);
        i ++;
    }
    free(namelist);
    
}

/**
 * Escribe la cantidad de archivos del directorio, en un archivo 
 * @param : filename : nombre del archivo
 * @param : n : cantidad por escribir en el archivo
 **/
void writeFileNumber(char * filename, int n)
{
    FILE *in = fopen(filename, "w");
    fprintf(in, "%i\n", n); 
    fclose(in);
}

/**
 * Obtiene la cantidad de archivos en un directorio, en tiempo dado, del archivo
 * @param : filename : nombre del archivo 
 * @return : la catidad de archivos - 0 si no existe el archivo.
 **/
int readFileCount(char *filename)
{
    int n;
    FILE *in = fopen(filename, "r");
    if (in != NULL)
    {
        fscanf(in, "%i", &n); 
        fclose(in);
        return n; 
    }
    return 0 ; 
}

/**
 *  Registra el estado actual del directorio
 **/
void registerFiles(char *directory, Array *files)
{
    struct dirent **namelist;
    int n;
    n = scandir(directory, &namelist, &filter, alphasort);
    if (n > 0)
    {
        initArray(files, n);
        scanFilesFromDirectory(files, namelist, n, directory);
        saveToFile(".meta/files_data.bin", files);
        writeFileNumber(".meta/count.bin", n);    
    }
    else
    {
        writeFileNumber(".meta/count.bin", 0);  
    }
    
}

/**
 * Realiza la comparación del directorio actual, con un listado anterior
 **/
 void compare(char *directory, Array *added_files, Array *modified_files, Array *deleted_files)
 {
    //  Iniciar los arreglos de entrada
    initArray(deleted_files, 1);
    initArray(added_files, 1);
    initArray(modified_files, 1);
    
    //  Obtener el listado anterior
    Array files, current_files ;
    int n = readFileCount(".meta/count.bin");
    initArray(&files, n);
    readFromFile(".meta/files_data.bin", &files);
    
    //  Obtener los archivos actuales
    struct dirent **namelist;
    int np = scandir(directory, &namelist, &filter, alphasort);
    initArray(&current_files, np);
    scanFilesFromDirectory(&current_files, namelist, np, directory);
    
    //  Conocer  los archivos eliminados
    diff(&files, n, &current_files, np, deleted_files) ;
    
    //  Conocer los archivos agregados
    diff(&current_files, np, &files, n, added_files);
    
    //  Conocer los archivos modificados 
    diffModified(&files, n, &current_files, np, modified_files);
    
    //  Liberar archivos actuales y anteriores
    freeArray(&files) ;
    freeArray(&current_files) ;
 }
 
 /**
  * Realiza la comparación del directorio actual, con un listado anterior
  * buscando solo los archivos modificados.
  **/
void compare_modified(char *directory, Array *current_files, Array *files, Array *modified_files)
{
    initArray(modified_files, 1);
    
    int n = readFileCount(".meta/count.bin");
    initArray(files, n);
    readFromFile(".meta/files_data.bin", files);
    
    //  Obtener los archivos actuales
    struct dirent **namelist;
    int np = scandir(directory, &namelist, &filter, alphasort);
    initArray(current_files, np);
    scanFilesFromDirectory(current_files, namelist, np, directory);
    
    //  Conocer los archivos modificados 
    diffModified(files, n, current_files, np, modified_files);
}



//  FUNCIONES UTILITARIAS PARA GENERACIÓN ALEATORIO DE NOMBRES DE ARCHIVOS
// =============================================================================


/**
 * Genera una hilera aleatorio
 **/
char *gen_string(int length) 
{    
    unsigned int key = 0, n;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-#87###";
    size_t stringLen = 69;        
    char *randomString;
    
    /* Intializes random number generator */
    //srand((unsigned) time(&t));

    randomString = malloc(sizeof(char) * (length +1));

    if (!randomString) return (char*)0;
    
    for ( n = 0; n < length; n++) 
    {            
        key = rand() % stringLen;          
        randomString[n] = string[key];
    }

    randomString[length] = '\0';

    return randomString;
}

/**
 * Inicializa en el sistema la semilla de generación de números random.
 **/ 
void init_random_seed()
{
    srand(time(NULL));
}

/**
 * Genera un nuevo nombre con semilla aleatorio para un archivo en un directorio dado.
 **/
void generateNewName(char *directory, char *oldname, char *newname)
{
    char *seed = gen_string(3) ;
    strcat(newname, directory) ;
    strcat(seed, oldname) ;
    strcat(newname, seed);
    free(seed) ;
}
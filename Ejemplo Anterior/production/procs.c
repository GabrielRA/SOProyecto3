
#include "array.h"


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
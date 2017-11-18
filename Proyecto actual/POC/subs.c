#include <stdio.h>
#include <malloc.h>

/**
* The following function excludes values from an array using another arrays values.
* Note that this version won't exclude multiple values, for this you have to drop
* '++j' in line 25.
*
* \param[in] from Original sorted array
* \param[in] from_length Original array length
* \param[in] what Sorted array including the excluding values
* \param[in] what_length self describing
* \param[out] result_length the lenght of the new array - a value lesser 0 indicates an error.
*/

int* exclude(int* from, int from_length, int* what, int what_length, int* result_length){
    int i,j,k;
    int* result = (int*) malloc(sizeof(int)*from_length);
    if(result == NULL){
        *result_length = -1;
        return NULL;
    }
    for(i = j = k = 0; i < from_length && j < what_length;){
        if(from[i] == what[j])
            ++i, ++j;  /* exclude this element - to enable multiple exclusion drop '++j' 
                        4,4,5,6 /4 --> 5,6 */
        if(from[i] < what[j])
            result[k++] = from[i++];
        if(from[i] > what[j])
             ++j; /* ignore lesser elements */
    }
    while(i < from_length)
        result[k++] = from[i++];

    if( k < from_length){
        int* tmp = (int*) realloc(result,sizeof(int)*k);
        if(tmp == NULL){
            /* either error handling or returning result */
        }else{
            result = tmp;
        }
    }
    *result_length = k;
    return result;
}

int main(){
    
    int a[6] = {1,2,3,4,5,6};
    int b[3] = {2,4,5};
    
    
    /*
    int a[3] = {1,2,3};
    int b[6] = {4,5,6,7,8,9};
    */
    
    int result_length;
    int i;
    int *c = exclude(a,6,b,3,&result_length);
    //int *c = exclude(b,3,a,6,&result_length);
    
    for(i = 0; i < result_length; ++i)
        printf("%i ",c[i]);
    free(c);
    return 0;
}
#include "array.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct ArrayData *array;
	array = initArray();

	int get;

	int i;
	for (i = 0; i < 4; i++)
	{
		addElement(array, 5);
		//get = getElement(array, i);
  		//printf("%d\n", get);
	}
    
    	printf("%d\n", getElement(array, 0));
    
	free(array->pointer);
	free(array);
	return 0;
}
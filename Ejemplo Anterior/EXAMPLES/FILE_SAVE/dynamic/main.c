
// https://stackoverflow.com/questions/35905566/storing-dynamic-arrays-of-a-struct-in-text-file-in-c-language
#include <stdio.h>
#include <stdlib.h>


struct car
{
    char name[20];
    char color[20];
    float mass;
    int price;
};

typedef struct car Cars;


int main(void)
{
    int i, n;
    Cars *cars;
    
    //  Read from file 
    FILE *in = fopen("cars_in.txt", "r");
    fscanf(in, "%i", &n);
    cars = (Cars *) malloc(n * sizeof(Cars));
    
    for (i = 0; i < n; ++i)
    {
        fscanf(in, "%s", cars[i].name);
        fscanf(in, "%s", cars[i].color);
        fscanf(in, "%f", &cars[i].mass);
        fscanf(in, "%i", &cars[i].price);
    }

    printf("%i\n", n);
    printf("%s\n", cars[0].name);
    printf("%s\n", cars[0].color);
    
    fclose(in);
    free(cars);
    cars = NULL;
    
    
    return 0;
}
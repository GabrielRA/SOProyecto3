#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct emp
{
    char name[15];
    int age;
    int salary;
    char address[30];
};

int main(void)
{
    char const filename[] = "sampledata.bin";
    struct emp emp1[5] =
    {
        { "Yoda",  23, 45000, "Asia"      },
        { "Darth", 34,  2344, "N America" },
        { "Jabba", 22,  5566, "Africa"    },
        { "Luke",  33,  3399, "S America" },
        { "Leia",  44,  6677, "Europe"    },
    };
    struct emp emp2[5];
    FILE *ifp;
    FILE *ofp;
    int i;
/*
    ofp = fopen(filename, "wb");
    if (ofp != 0)
    {
        if (fwrite(emp1, sizeof(emp1), 1, ofp) != 1)
        {
            fprintf(stderr, "Failed to write to %s\n", filename);
            exit(1);
        }
        fclose(ofp);
    }
*/
    ifp = fopen(filename, "rb");
    if (ifp != 0)
    {
        if (fread(emp2, sizeof(emp2), 1, ifp) != 1)
        {
            fprintf(stderr, "Failed to read from %s\n", filename);
            exit(1);
        }
        fclose(ifp);
    }

    for (i = 0; i < 5; i++)
    {
        /*
        if (emp1[i].age != emp2[i].age ||
            emp1[i].salary != emp2[i].salary ||
            strcmp(emp1[i].name, emp2[i].name) != 0 ||
            strcmp(emp1[i].address, emp2[i].address) != 0)
            printf("Difference in record %d\n", i);
        */
        printf("%s\n", emp2[i].name);
    }

    return 0;
}
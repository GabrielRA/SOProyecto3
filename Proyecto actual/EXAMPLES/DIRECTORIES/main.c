#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <langinfo.h>

#include <strings.h>
#include <sys/stat.h>
#include <dirent.h>


struct dirent **namelist;
int n;

int alphabeticCompare(void* a, void* b)
{
    int res;
    
    res = strcasecmp( ( *(struct dirent **) b ) -> d_name,
                      ( *(struct dirent **) a ) -> d_name);
                      
    return res;
}

int filter(const struct dirent * dir)
{
    const char *s = dir -> d_name;
    if (s[0] == '.') return 0;
    return 1;
}



int main(void)
{
   struct       dirent **namelist;
   struct       stat statBuffer;
   struct       tm *tm;
   char         datestring[256];
   int          n;

   n = scandir(".", &namelist, &filter, alphasort);
   if (n < 0) perror("scandir");
   else
   {
       while (n--)
       {
           
           //   Tomar el dato del archivo
           if (stat(namelist[n] -> d_name, &statBuffer) == -1) continue;
           
           printf("%d ", namelist[n]->d_type);
           printf("%s", namelist[n]->d_name);
           tm = localtime(&statBuffer.st_mtime);
           strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
           printf(" %s\n", datestring);
           free(namelist[n]);
           
           printf("%i ", tm -> tm_sec);
       }
       free(namelist);
   }
   
   exit(0);
}
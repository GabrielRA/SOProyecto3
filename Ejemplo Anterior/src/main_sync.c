
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.c"
#include "client.c"
#include "structs.h"


void init_random_seed();


void actLikeClient(char *directory, char *hostname)
{
    printf("CLIENTE ======== directorio: %s , y el servidor: %s \n", directory, hostname);
    init_client(hostname, directory);
}

void actLikeServer(char *directory)
{
    printf("SERVIDOR ======== directorio: %s \n", directory);
    init_server(directory) ;
}


int main(int argc, char **argv)
{
	char ch;
    char *host = NULL, *dir = NULL;

	 while((ch = getopt(argc, argv, "h:d:")) != -1) {
        switch (ch) 
        {
            case 'h':
                host = optarg;
                break;
            case 'd':
                dir = optarg;
                break;
            default:
                fprintf(stderr, "Uso: CLIENT -> ./program -h <server hostname> -d <directory>\n SERVER -> ./program -d <directory> \n");
                exit(1);
        }
    }
    
    createDirectory(".meta/");
    init_random_seed() ;
    
    //  Decidir el rol del programa
	if(!host && dir) 
	{
	    actLikeServer(dir);
	    
	} else if (host && dir)
	{
	    actLikeClient(dir, host) ;
	}
	else
	{
	    fprintf(stderr, "Error \n Uso: CLIENT -> ./program -h <server hostname> -d <directory>\n SERVER -> ./program -d <directory> \n");
	    exit(1);
	}
	
	return 0 ;
	
    
}
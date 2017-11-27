
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "client.c"
#include "server.c"

/*

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
}*/


int main(int argc, char **argv)
{
	char ch;
    char *host = NULL, *dir = NULL;

    printf("\nEjecutando el sistema BASIC FTP...\n\n");

    char open, close, quit, cd, get,lcd,ls,put,pwd;
    
    // parse options
    printf("--- Cantidad de parámetros en la entrada: %d\n\n", argc );

    int chosen = 0;
    if (argc == 2){ 

        if (strcmp(argv[1], "open") == 0){
            open = 1;
            chosen = 1;
            printf("---------->  Abriendo una nueva conexión\n\n");
        }
        if (strcmp(argv[1], "close") == 0){
            close = 1;
            chosen = 1;
            printf("---------->  Cerrando una nueva conexión\n\n");
        }
        if (strcmp(argv[1], "quit") == 0){
            quit = 1;
            chosen = 1;
            printf("---------->  Saliendo del sistema\n\n");
        }
        if (strcmp(argv[1], "get") == 0){
            get = 1;
            chosen = 1;
            printf("---------->  Recibiendo un archivo de una conexión remota\n\n");
        }
        if (strcmp(argv[1], "lcd") == 0){
            lcd = 1;
            chosen = 1;
            printf("---------->  Cambiando de directorio local\n\n");
        }
        if (strcmp(argv[1], "put") == 0){
            put = 1;
            chosen = 1;
            printf("---------->  Enviando un archivo a una conexión remota\n\n");
        }
        if (strcmp(argv[1], "ls") == 0){
            ls = 1;
            chosen = 1;
            printf("---------->  Enlistando los archivos del directorio remoto\n\n");
        }
        if (strcmp(argv[1], "pwd") == 0){
            pwd = 1;
            chosen = 1;
            printf("---------->  Mostrando el directorio activo remoto\n\n");
        }
    }
    else
    {
        if (chosen == 0){     //printf("Se ha digitado una opción inválida\n");
            printf("    INSTRUCCIONES QUE SE PUEDEN EJECUTAR\n"\
            "   -----------------------------------\n"\
            "   open:      Establecer una conexión remota\n"\
            "   close:     Cerrar la conexión actual\n"\
            "   quit:      Terminar el programa\n"\
            "   cd:        Cambiar de directorio remoto\n"\
            "   get:       Recuperar un archivo remoto\n"\
            "   lcd:       cambiar de directorio local\n"\
            "   ls:        listar los archivos del directorio remoto\n"\
            "   put:       enviar un archivo a la màquina remota\n"\
            "   pwd:       mostrar el directorio del archivo remoto\n");
        }
    }




/*

            case 'c': close = 1; break;             
            case 'q': quit = 1; break; 
            case 'd': cd = 1; break; 
            case 'g': get = 1; break; 
            case 'l': lcd = 1; break; 
            case 's': ls = 1; break; 
            case 'p': put = 1; break; 
            case 'w': pwd = 1; break;
            default:
                fprintf(stderr, "Error: Bad option: %s\n", argv[1]);
                fprintf(stderr, "Do '%s help' for help\n", argv[0]);
                return 0;
                break;
        }
    }*/

    /*

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
    */
    
    //createDirectory(".meta/");
    //init_random_seed() ;
    
    //Decidir el rol del programa

    /*
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
    */
	
	return 0 ;
	
    
}
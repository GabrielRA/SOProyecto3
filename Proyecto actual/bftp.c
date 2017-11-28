
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "client.c"



int main(int argc, char **argv){
	char ch;
    char *host = NULL, *dir = NULL;

    printf("\nEjecutando el sistema BASIC FTP...\n\n");

    char open, close, quit, cd, get,lcd,ls,put,pwd;
    
    // parse options
    printf("--- Cantidad de parámetros en la entrada: %d\n\n", argc );

    int chosen = 0;
    if (argc >= 2){ 

        if (strcmp(argv[1], "open") == 0){
            open = 1;
            chosen = 1;
            if (argc == 4){
                printf("---------->  Abriendo una nueva conexión\n\n");
                printf("    1. Realizar una conexión\n");
                printf("    2. Esperar una conexión\n\n");
                printf("Digite una opción a ejecutar: ");
                int option = 0;
                scanf("%d", &option);

                printf("La opción digitada es: %d\n\n", option);
                switch(option){
                    case 1:
                    host = argv[2];
                    dir = argv[3];
                    init_client(host, dir);
                    break;

                    case 2:
                    dir = argv[2];
                    init_server(dir);
                    break;

                    default:
                    printf("Digite una opción correcta\n");
                }
            }
            else{
                printf("No se ha podido realizar la conexión. Datos incorrectos o insuficientes\n\n"\
                    "   ./bftp open <host server> <directory>\n\n");
            }
        }
        if (strcmp(argv[1], "close") == 0){
            close = 1;
            chosen = 1;
            if (argc == 3){
                printf("---------->  Cerrando una conexión\n\n");
                dir = argv[2];
                init_server(dir);
            }
        }
        if (strcmp(argv[1], "quit") == 0){
            quit = 1;
            chosen = 1;
            if (argc >= 3){
                printf("---------->  Saliendo del sistema\n\n");
            }
        }
        if (strcmp(argv[1], "get") == 0){
            get = 1;
            chosen = 1;
            printf("---------->  Recibiendo un archivo de una conexión remota\n\n");
            //get()
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

    return 0;
}
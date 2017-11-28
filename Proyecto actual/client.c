
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "structs.h"
#include <dirent.h>
#include "base64.c"


//  PROTOTIPOS DE LAS FUNCIONES POR USAR =======================================

void get(int soc, struct sync_file_message received_packet, int filesize) ;
void put(int soc, char* filename, int size) ;
void Writen(int fd, void *ptr, size_t nbytes) ;
ssize_t Readn(int fd, void *ptr, size_t nbytes) ;

void scanFilesFromDirectory(Array *files, struct dirent **namelist, int n, char *directory) ;
void writeFileNumber(char * filename, int n) ;
void registerFiles(char *directory, Array *files) ;
void compare(char *directory, Array *added_files, Array *modified_files, Array *deleted_files) ;
void generateNewName(char *directory, char *oldname, char *newname);

int readFileCount(char *filename) ;
void registerFiles(char *directory, Array *files) ;

void compare_modified(char *directory, Array *current_files, Array *files, Array *modified_files) ;
// =============================================================================

/**
 * Crea un socket e intenta conectarse al servidor por medio del hostname.
 * @Param : hostname : String - Nombre del host por conectarse
 * @Return : el indentificador del socket, al cuál se realizó la conexión.
 **/
int connect_to_server(char *hostname)
{
    int sock ;
    struct sockaddr_in server;
    
    //  Crear el socket
    sock = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sock == -1) return -1 ;
    
    server.sin_addr.s_addr = inet_addr(hostname);
    server.sin_family = AF_INET;
    server.sin_port = htons( 8889 );
    
    //  Conectarse al servidor
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) 
    {
        printf("Error de conexión\n") ;
        return -1 ;
    }
    
    return sock ;
}



/**
 * Configura el socket del servidor. Crea y asocia el socket a un puerto.
 * @return: listenfd (descriptot del socket) donde se está escuchando al cliente
 **/
int setup()
{
    int socket_descriptor ;
    struct sockaddr_in server ;
    
    //  Crear el socket 
    socket_descriptor = socket(AF_INET, SOCK_STREAM , 0) ;
    if (socket_descriptor == -1) return socket_descriptor ;
    
    //  Preparar la estructura sockaddr_in
    server.sin_family = AF_INET ;
    server.sin_addr.s_addr = INADDR_ANY ;
    server.sin_port = htons ( 8889 ) ;
    
    //  Ligar
    if( bind(socket_descriptor,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        return -1 ;
    }
    
    //  Escuchar
    listen(socket_descriptor , 3) ;
    
    return socket_descriptor ;
}

/**
 * Realiza la inicialización del lado del cliente.
 * @param : directory : nombre del directorio que se desea sincronizar
 * @return : codigo de éxito en la comunicación
 **/
int init_client(char *hostname, char *directory)
{
    int sock ;
    int ret = 0;
    if (ret = authenticate() == 0) {

        
        sock = connect_to_server(hostname) ;
        
        if (sock < 0)
        {
            printf("--- No se pudo conectar con el otro cliente\n") ; 
            printf("--- Verifique que el otro cliente esté conectado\n\n");
        }
        else{
            printf("--- El cliente se conectó\n") ;

            printf("--- Se realiza la conexión con éxito\n");
        
        }
    }
    else{
        printf("--- No se ha logrado una correcta autenticación\n"\
            "Intente con un usuario y contraseña registrado\n\n");
    }
    
    
    close(sock) ;
    return 0 ;
}


/**
 * Realiza la inicialización del lado del servidor.
 * @param : directory : nombre del directorio que se desea sincronizar
 * @return : codigo de éxito en la comunicación
 **/
int init_server(char *directory)
{
    //  Crear el directorio
    createDirectory(directory) ;
    
    int client_socket , c ;
    struct sockaddr_in client ;
    int socket_desc = setup() ;
    
    if (socket_desc < 0)
    {
        perror ("No se pudo crear el socket del server") ;
        return -1 ;
    }
    else
    {
        printf("\n\n¡Se ha conectado con un nuevo cliente. Estoy en espera de recibir o enviar archivos \n") ;
        c = sizeof(struct sockaddr_in) ;
        client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) ;
        if (client_socket < 0)
        {
            perror("No se pudo aceptar al cliente") ;
            return -1 ;
        }
        printf("Conexión aceptada \n") ;
        
        close(client_socket) ;        
    }
    return 0 ;
}
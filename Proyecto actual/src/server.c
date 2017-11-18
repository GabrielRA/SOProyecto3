
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <dirent.h>

#include "structs.h"

//  PROTOTIPOS DE LAS FUNCIONES POR USAR =======================================
void get_file(int soc, struct sync_file_message received_packet, int filesize) ;
void send_file(int soc, char* filename, int size) ;
void Writen(int fd, void *ptr, size_t nbytes) ;
ssize_t Readn(int fd, void *ptr, size_t nbytes) ;

int readFileCount(char *filename) ;
void registerFiles(char *directory, Array *files) ;

void compare_modified(char *directory, Array *current_files, Array *files, Array *modified_files) ;
void generateNewName(char *directory, char *oldname, char *newname);

// =============================================================================

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
 * Esta función recibe un conjunto de archivos.
 * Se debe ejecutar en caso del directorio actual vacío y se debe recibir multiples archivos
 * @param : client_socket : socket con la conexión al cliente.
 **/
void receive_all_files(int client_socket)
{
    int filesize ;
    struct sync_file_message received_packet ;
    for (; ;)
    {
        int n = Readn(client_socket, &received_packet, sizeof(received_packet));

        if (n > 0)
        {
            printf("Nombre del archivo %s y su tamaño %i \n", received_packet.filename, received_packet.size) ;
            filesize = received_packet.size ;
        }
        else if (n == 0)
        {
            puts("Cliente desconectado");
	        fflush(stdout);
	        break;
        }
        //  Recibir los archivos del cliente
        get_file(client_socket, received_packet, filesize) ; 
        
    }
}

/**
 * Esta función procesa las solicitudes de cambio obtenidas desde el cliente.
 * Se debe ejecutar en caso que haya que sincronizar un directorio NO vacío del servidor
 * @param : client_socket : socket con la conexión al cliente.
 * @param : directory : directorio que se desea procesar y sincronizar
 **/
void process_file_changes(int client_socket, char  *directory)
{
    struct sync_message received_packet ;

    for (; ;)
    {
        int n = Readn(client_socket, &received_packet, sizeof(received_packet));
        if (n > 0)
        {
            if (received_packet.added_file == 1) 
            {
                printf("AGREGANDO el archivo %s de tamaño %i \n", received_packet.message, received_packet.size) ; 
                int filesize = received_packet.size ;
                struct sync_file_message received_file ;
                strncpy(received_file.filename, received_packet.message, 1000);
                get_file(client_socket, received_file, filesize) ; 
            }
            else if (received_packet.deleted_file == 1) 
            {
                printf("ELIMINANDO el archivo %s\n", received_packet.message) ;
                remove(received_packet.message) ;
            }
            else if (received_packet.modified_file == 1) 
            {
                //  Obtener el listado anterior
                Array files, current_files, modified_files ;
                
                compare_modified(directory, &current_files, &files, &modified_files) ;

                printf("MODIFICANDO el archivo %s\n", received_packet.name);
                
                int index = findArray(&current_files, current_files.used, received_packet.name) ;
                file_data file = current_files.array[index] ;
                
                if ((index = findArray(&modified_files, modified_files.used, received_packet.name)) >= 0)
                {
                    file.modification_time = 0 ;
                }
                //  Enviar el dato del archivo analizado al ciente
                Writen(client_socket, &file, sizeof(file)) ;
                
                //  Verificar que acciones hay que tomar 
                if (file.modification_time == 0)
                {
                    char newname[1000];
                    generateNewName(directory, modified_files.array[index].name, newname) ;
                    rename( modified_files.array[index].path, newname) ;
                    
                    struct sync_file_message received_packet, m;
                    m.size = modified_files.array[index].size;
                    strncpy(m.filename, newname, 1000);
                    
                    int n = Readn(client_socket, &received_packet, sizeof(received_packet));
                    if (n > 0)
                    {
                       printf("El archivo %s está cambiado en ambas partes, y su nombre en el server es: %s \n", modified_files.array[index].name, newname) ; 
                       printf("El nombre en el cliente es: %s y su tamaño es %i\n", received_packet.filename, received_packet.size) ;
                       
                       m.size = modified_files.array[index].size;
                       strncpy(m.filename, newname, 1000);
                       Writen(client_socket, &m, sizeof(m));
                       
                       get_file(client_socket, received_packet, received_packet.size);
                       send_file(client_socket, newname, modified_files.array[index].size) ;
                    }
  
                }
                else if (received_packet.mtime > file.modification_time)
                {
                    struct sync_file_message f ;
                    strncpy(f.filename, received_packet.message, 1000) ;
                    fopen(received_packet.message,"wb") ;
                    get_file(client_socket, f, received_packet.size) ;
                }
                else if (received_packet.mtime  < file.modification_time)
                {
                    printf("El servidor tiene el más reciente que el cliente\n");
                }
                
                
                freeArray(&files) ;
                freeArray(&current_files) ;
                freeArray(&modified_files) ;
            }
        }
        else if (n == 0)
        {
            puts("Cliente desconectado");
	        fflush(stdout);
	        break;
        }
    }
    
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
        printf("¡Hola!, soy el server esperando por conexiones \n") ;
        c = sizeof(struct sockaddr_in) ;
        client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) ;
        if (client_socket < 0)
        {
            perror("No se pudo aceptar al cliente") ;
            return -1 ;
        }
        printf("Conexión aceptada \n") ;
        
        
        //  Iniciar la comunicación con el client
        Array files ;
        struct sync_message handshake , response ;
       
        int n = Readn(client_socket, &handshake, sizeof(handshake)) ;
        int cant_files = 0 ;
        if (n > 0)
        {
            //  Corroborar si el servidor está vacío
            cant_files = readFileCount(".meta/count.bin");
            if (cant_files == 0) response.empty_directory = 1 ;
            else response.empty_directory = 0 ;
            
            //  Avisar al cliente que el servidor está vacío y listo para recibir todos los archivos
            Writen(client_socket, &response, sizeof(response));
        }
        
        //  Como el directorio está vacío, recibo todos los archivos del cliente
        if (cant_files == 0) receive_all_files(client_socket) ;
        //  El directorio del server no está vacío, así que hay que verificar cambios
        else if (cant_files > 0) process_file_changes(client_socket, directory) ;
        
        //  Registrar el estado en el que queda el directorio
        registerFiles(directory, &files) ;
        close(client_socket) ;
        freeArray(&files) ;
        
        return 0 ;
    }
}
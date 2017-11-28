
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "structs.h"
#include <dirent.h>


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
 * Obtiene los archivos actuales del directorio. Se verifica si ya existe un
 * archivo de metadata con datos de archivos anteriores almacenados.
 **/

/*
void get_directory_files(char *directory, Array *files)
{
    int n = readFileCount(".meta/count.bin");
    
    if (n > 0)
    {
        initArray(files, n);
        readFromFile(".meta/files_data.bin", files);
    }
    else
    {
        registerFiles(directory, files);
    }
}*/


/**
 * Esta función envia un conjunto de archivos al servidor.
 * Se debe ejecutar en caso del directorio actual vacío y se debe transmitir multiples archivos
 * @param : socket : socket con la conexión al servidor.
 **/

/*
void send_all_files(int socket, char *directory)
{
    Array files ;
    
    struct stat fileStat;
        
    get_directory_files(directory, &files) ;
    
    int i;
    for (i = 0; i < files.size; i++)
    {
        if(stat(files.array[i].path, &fileStat) == 0) 
        {
            struct sync_file_message m;
            m.size = fileStat.st_size;
            strncpy(m.filename, files.array[i].path, 1000);
            
            //  Enviar datos básicos sobre el archivo antes - nombre y tamaño
            Writen(socket, &m, sizeof(m));
            
            //  Se envía el contenido del archivo al servidor
            put(socket, files.array[i].path, fileStat.st_size); 
        }
    }
    
    freeArray(&files);
}
*/

/*

//  Procesar los archivos eliminados
void process_deleted_files(int socket, Array *deleted_files)
{
    int i ;
    
    for (i = 0; i < deleted_files->used; i ++)
    {
        struct sync_message sync ;
        sync.deleted_file = 1 ;
        strncpy(sync.message, deleted_files->array[i].path, 1000);
        Writen(socket, &sync, sizeof(sync));
        sync.deleted_file = 0 ;   
        
    }
    
}
*/

/*
//  Procesar los archivos agregados
void process_added_files(int socket, Array *added_files)
{
    int i ;
    for (i = 0; i < added_files->used; i ++)
    {
        struct sync_message sync ;
        struct stat fileStat;
        
        if(stat(added_files->array[i].path, &fileStat) == 0) 
        {
            sync.added_file = 1 ;
            sync.size = fileStat.st_size ;
            strncpy(sync.message, added_files->array[i].path, 1000) ;
            Writen(socket, &sync, sizeof(sync)) ;
            sync.added_file = 0 ;
            
            // Enviar el archivo agregado al servidor
            put(socket, added_files->array[i].path, fileStat.st_size); 
        }
        
    }
}*/

/*
//Procesar los archivos modificados
void process_modified_files(int socket, Array *modified_files, char *directory)
{
    int i ;
    for (i = 0; i < modified_files->used; i ++)
    {
        struct sync_message sync ;
        file_data file ;
        
        //  Preparar los datos del archivo modificado para ser enviados al servidor
        strncpy(sync.name, modified_files->array[i].name, 1000);
        strncpy(sync.message, modified_files->array[i].path, 1000);
        sync.mtime = modified_files->array[i].modification_time ;
        sync.size = modified_files->array[i].size ;
        
        //  Enviar los datos del archivo modificado al servidor
        sync.modified_file = 1 ;
        Writen(socket, &sync, sizeof(sync)) ;
        sync.modified_file = 0 ;
        
        
        //  Obtener la respuesta del servidor
        int n = Readn(socket, &file, sizeof(file));
       
        if (file.modification_time == 0)
        {
            char newname[1000];
            generateNewName(directory,modified_files->array[i].name, newname) ;
            rename(modified_files->array[i].path, newname) ;
            //printf("El archivo está cambiado en ambas partes y su nuevo nombre en el cliente es %s\n", newname) ;
            struct sync_file_message m, received_packet;
            m.size = modified_files->array[i].size;
            strncpy(m.filename, newname, 1000);
            
            Writen(socket, &m, sizeof(m));
            
            n = Readn(socket, &received_packet, sizeof(received_packet));
            if (n > 0)
            {
                printf("El nombre en el servidor es: %s y su tamaño es %i\n", received_packet.filename, received_packet.size) ;
                
                put(socket, newname, modified_files->array[i].size) ;
                get(socket, received_packet, received_packet.size);
            }
        }
        else if (modified_files -> array[i].modification_time > file.modification_time)
        {
            printf("El cliente tiene el más reciente que el servidor\n");
            put(socket, modified_files->array[i].path, modified_files->array[i].size ) ;
        }
        else if (modified_files->array[i].modification_time < file.modification_time)
        {
            printf("El servidor tiene el màs reciente que el cliente\n");
        }
    }
}*/


/**
 * Realiza la inicialización del lado del cliente.
 * @param : directory : nombre del directorio que se desea sincronizar
 * @return : codigo de éxito en la comunicación
 **/
int init_client(char *hostname, char *directory)
{
    int sock ;
    
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
    //  Iniciar la comunicación con el servidor

    /*struct sync_message handshake ;
    struct sync_message response;
    strncpy(handshake.message, "Sincronizar archivos", 900);
    
    Writen(sock, &handshake, sizeof(handshake)) ;
    
    int n = Readn(sock, &response, sizeof(response)) ;
   
    if (n > 0 && response.empty_directory == 1)
    {
        send_all_files(sock, directory) ;
    }
    else
    {
        printf("Procesando los archivos del cliente\n") ;
        
        Array deleted_files, modified_files, added_files ;
        
        compare(directory, &added_files, &modified_files, &deleted_files) ;
    
        if (added_files.used > 0) process_added_files(sock, &added_files) ;
        if (deleted_files.used > 0) process_deleted_files(sock, &deleted_files) ;
        if (modified_files.used > 0) process_modified_files(sock, &modified_files, directory) ;
        if (added_files.used == 0 && modified_files.used == 0 && added_files.used == 0) printf("NO hay nuevos cambios en el directorio %s \n", directory) ;
        
        freeArray(&deleted_files);
        freeArray(&added_files);
        freeArray(&modified_files);
    }
    */
    
    //  Registrar el estado en el que queda el directorio
    //Array files;
    //registerFiles(directory, &files) ;
    
    close(sock) ;
    //freeArray(&files) ;
 
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
        
        
        //  Iniciar la comunicación con el client
        /*
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
        
        /*
        //  Como el directorio está vacío, recibo todos los archivos del cliente
        if (cant_files == 0) receive_all_files(client_socket) ;
        //  El directorio del server no está vacío, así que hay que verificar cambios
        else if (cant_files > 0) process_file_changes(client_socket, directory) ;
        
        //  Registrar el estado en el que queda el directorio
        registerFiles(directory, &files) ;
        */
        close(client_socket) ;
        //freeArray(&files) ;
        
        
    }
    return 0 ;
}
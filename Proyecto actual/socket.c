
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include "structs.h"


void get_file(int soc, struct sync_file_message received_packet, int filesize) ;
void send_file(int soc, char* filename, int size) ;
ssize_t	writen(int fd, const void *vptr, size_t n) ;
void Writen(int fd, void *ptr, size_t nbytes) ;
ssize_t readn(int fd, void *vptr, size_t n) ;
ssize_t Readn(int fd, void *ptr, size_t nbytes) ;

// ==== OPERACIONES PARA PROCESAR ARCHIVOS TRANSMITIDOS POR MEDIO DE LOS SOCKETS

/**
 * Recibe el contenido de un archivo enviado a través de una conexión.
 * soc : socket del cuál recibir el archivo
 * received_packet : paquete recibido con el bufer con el contenido del archivo.
 * filesize : tamaño del archivo por recibir.
 **/
void get_file(int soc, struct sync_file_message received_packet, int filesize)
{
    struct flock lock ;
    int fd ;
    
    int j = 0 ;
    FILE *fp = fopen(received_packet.filename,"ab") ;
    if (fp != NULL) 
    {
        // Colocar el bloque al archivo
        fd = fileno(fp) ;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_SETLKW;
        fcntl (fd, F_SETLKW, &lock);
        
        while ( j < filesize )
        {
            int bytesReceived = Readn(soc, &received_packet, sizeof(received_packet ) ) ;
            if (bytesReceived == 0) break ;
            j += 1024 ;
            fwrite( received_packet.fileBuff, 1, received_packet.size, fp ) ;
            
        }
        // Quitar el bloqueo al archivo
        lock.l_type = F_UNLCK;
        fcntl (fd, F_SETLKW, &lock);
        
        fclose(fp);
    }
    else
    {
        printf("ERROR al abrir el archivo %s \n", received_packet.filename) ;
    }
    
}

/**
 * Envía un el contenido de un archivo, por medio del socket soc.
 * @soc : conexión del socket al cuál se enviará el contenido del archivo 
 * @filename : nombre del archivo por enviar
 * @size : Tamaño del archivo por enviar.
 **/
void send_file(int soc, char* filename, int size)
{
    struct flock lock ;
    int fd ;
    struct sync_file_message m;
    strncpy(m.filename, filename, 1000);
    int j = 0, bytesRead = 0;
    FILE *fp = fopen(filename,"rb");
    
    
    if (fp != NULL)
    {
        // Colocar el bloque al archivo
        fd = fileno(fp) ;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_SETLKW;
        fcntl (fd, F_SETLKW, &lock);
        
        while (j < size)
        {
            bytesRead = fread(m.fileBuff, 1, 1024, fp) ;
            m.size = bytesRead ;
            Writen(soc, &m, sizeof(m)); 
            j += 1024;
        }
        
        // Quitar el bloqueo al archivo
        lock.l_type = F_UNLCK;
        fcntl (fd, F_SETLKW, &lock);
        
        fclose(fp);
    }
}


// ======== OPERACIONES PARA ESCRIBIR Y LEER DATOS SOBRE LOS SOCKETS ===========

/* Escribe "n" bytes hacia un descriptor del socket. */
ssize_t	writen(int fd, const void *vptr, size_t n)
{
    size_t		nleft;
    const char	*ptr;
    ssize_t		nwritten;
    ptr = vptr;
    nleft = n;
    while (nleft > 0) 
    {
    	if ( (nwritten = write(fd, ptr, nleft)) <= 0) 
    	{
    	    if (errno == EINTR)
    		nwritten = 0;		/* para volver a llamar write() */
    	    else return(-1);		/* error */
    	}
    	nleft -= nwritten;
    	ptr   += nwritten;
    }
    return(n);
}

void Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
	perror("writen error");
}

/* Lee "n" bytes desde el descriptot del socket */
ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t	nleft;
    ssize_t	nread;
    char	*ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0)
    {
    	if ( (nread = read(fd, ptr, nleft)) < 0) 
    	{
    	    if (errno == EINTR)
    		nread = 0;		/* volver a llamar read() */
    	    else
    		return(-1);
    	} else if (nread == 0)
    	    break;				/* EOF */
    	
    	nleft -= nread;
    	ptr   += nread;
    }
    return(n - nleft);		/* return >= 0 */
}

ssize_t Readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;
    if ( (n = readn(fd, ptr, nbytes)) < 0)
	perror("readn error");
    return(n);
}

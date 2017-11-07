
#include<stdio.h>           //printf
#include<string.h>          //strlen
#include<sys/socket.h>      //socket
#include<arpa/inet.h>       //inet_addr

 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("No se pudo crear el socket cliente");
    }
    puts("Socket cliente creado");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Erro de conexión");
        return 1;
    }
     
    puts("Conectado\n");
     
    //keep communicating with server
    while(1)
    {
        printf("Ingrese el nombre del archivo : ");
        scanf("%s" , message);
        
        
        char buffer[100];
        FILE *f;
        f=fopen(message,"r");
        fscanf(f,"%s",buffer);
         
        //Send some data
       
        write(sock,buffer,100);

         
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
    }
     
    close(sock);
    return 0;
}
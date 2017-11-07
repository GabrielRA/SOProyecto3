/*
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <unistd.h>

  int main(int argc, char * argv[]) 
  {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];
   
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
      printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
    //Bind
    if (bind(socket_desc, (struct sockaddr * ) & server,
        sizeof(server)) < 0) {
      //print the error message
      perror("bind failed. Error");
      return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc, 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc,
      (struct sockaddr * ) & client, (socklen_t * ) & c);
    if (client_sock < 0) {
      perror("accept failed");
      return 1;
    }
    puts("Connection accepted");
    
    //read_size = read(client_sock, client_message, 256);
    //printf("Hola %s\n", client_message);
    
    //Receive a message from client
    while ((read_size = read(client_sock, client_message, sizeof(client_message)-1)) > 0) 
    {
      //Send the message back to client
      //printf(" Leí -> %i,  %s\n", read_size,client_message);

      printf(" Leí -> %i\n", read_size);
      write(client_sock, client_message, strlen(client_message));
      
    }
    if (read_size == 0) 
    {
      puts("Client disconnected");
      fflush(stdout);
    } else if (read_size == -1) 
    {
      perror("recv failed");
    }

    return 0;
  }
  */
  
#include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  
  int main(void)
  {
    struct sockaddr_in sa;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (SocketFD == -1) {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
  
    memset(&sa, 0, sizeof sa);
  
    sa.sin_family = AF_INET;
    sa.sin_port = htons(1100);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
  
    if (bind(SocketFD,(struct sockaddr *)&sa, sizeof sa) == -1) {
      perror("bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
  
    if (listen(SocketFD, 10) == -1) {
      perror("listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
  
    for (;;) {
      int ConnectFD = accept(SocketFD, NULL, NULL);
  
      if (0 > ConnectFD) {
        perror("accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
  
      /* perform read write operations ... 
      read(ConnectFD, buff, size)
      */
  
      if (shutdown(ConnectFD, SHUT_RDWR) == -1) 
      {
        perror("shutdown failed");
        close(ConnectFD);
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
      close(ConnectFD);
    }

    close(SocketFD);
    return EXIT_SUCCESS;  
}

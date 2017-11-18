#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


void* SendFileToClient(int *arg, char* filename)
{
    int connfd=(int)*arg;
    char message[1000] , server_reply[2000];
   
    //write(connfd, filename,256);


    FILE *fp = fopen(filename,"rb");
    if(fp==NULL)
    {
        printf("File open error");
    }   
    unsigned char buff[1024] = {0};
    int nread = fread(buff,1,1024,fp);
    
            
        if(nread > 0)
        {
            //printf("Sending \n");
            write(connfd, buff, nread);
        }
        if (nread < 1024)
        {
            if (feof(fp))
        	{
                printf("End of file\n");
        	    printf("File transfer completed for id: %d\n",connfd);
        	}
            if (ferror(fp))
                printf("Error reading\n");
            
        }
     
    
    
    
	if( read(connfd, server_reply, sizeof(server_reply) - 1) < 0) 
	{
		puts("recv failed - al leer del servidor");
		
	}
	
	printf("Server reply: %s \n", server_reply);
	
    

}


int main(int argc , char *argv[]) 
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
	printf("Could not create socket");
	}
	puts("Socket created");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server ,
	sizeof(server)) < 0) {
	perror("connect failed. Error");
	return 1;
	}
	puts("Connected\n");
	//keep communicating with server
	while(1) 
	{

		char *hola[] = {"hola.txt", "document.pdf"};
		int i;
		
		for (i = 0; i < 2; i++)
		{
			/*
			write(sock,hola[i], 255);
			
			//write(sock,"HOOOOOOOOOOLA", 500);
			//read(sock, server_reply, sizeof(server_reply) - 1);
			
			if( read(sock, server_reply, sizeof(server_reply) - 1) < 0) 
			{
				puts("recv failed - al leer del servidor");
				exit(1);
			}
			printf("Server reply: %s (enviados: %i) (recibidos : %i)\n", server_reply, (int)strlen(hola[i]), (int)strlen(server_reply));
			*/
			SendFileToClient(&sock,hola[i]);
		}
		close(sock);
		break;
		
		
		//Receive a reply from the server
		/*
		int n;
		if( (n = read(sock, server_reply, sizeof(server_reply)-1))< 0) 
		{
			puts("recv failed - al leer del servidor");
			break;
		}*/
		
		//puts("Server reply :");
		//puts(server_reply);
		//printf("Server reply %s\n", server_reply);
		//close(sock);
	}
	//close(sock);

	return 0;
}
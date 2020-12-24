#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>



int main(int argc, char *argv[])
{
    
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);// adresse du server
    
    char ip[50];
if (argc < 2) 
{
    printf("Enter IP address to connect: ");
    gets(ip);
}
else
  strcpy(ip,argv[1]);

    serv_addr.sin_addr.s_addr = inet_addr(ip);

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    printf("Connected to ip: %s : %d\n",inet_ntoa(serv_addr.sin_addr),ntohs(serv_addr.sin_port));

   	 // Création de fichier de sauvegarde des data
    FILE *fp;
	char fname[100];
	read(sockfd, fname, 256);

	printf("File Name: %s\n",fname);
	printf("Receiving file...");
   	 fp = fopen(fname, "ab"); 
    	if(fp == NULL)
    	{
       	 printf("Error opening file");
         return 1;
    	}
  
    /* Receive data in chunks of 256 bytes */
    while((bytesReceived = read(sockfd, recvBuff, 1024)) > 0)
    { 
        fwrite(recvBuff, 1,bytesReceived,fp);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
    printf("\nFile OK....Completed\n");
    return 0;
}

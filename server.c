#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>


struct sockaddr_in c_addr;
char fname[100];
void* SendFileToClient(int *arg);


void* SendFileToClient(int *arg)
{
      int connfd=(int)*arg;
 
       write(connfd, fname,256);

        FILE *fp = fopen(fname,"r");
        if(fp==NULL)
        {
            printf("File opern error");
            //return 1;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
      

            /* If read was success, send data. */
            if(nread > 0)
            {
                write(connfd, buff, nread);
                break;
            }
            if (nread < 1024)
            {
                if (feof(fp))
				{
                    printf("End of file\n");
		    printf("File transfer completed for id: %d\n",connfd);
				}
            } 
        }
        
printf("Closing Connection for id: %d\n",connfd);
close(connfd);
}

int main(int argc, char *argv[])
{
    int connfd = 0,err;
    pthread_t tid; 
    struct sockaddr_in serv_addr;
    int listenfd = 0,ret;
    char sendBuff[1025];
    int numrv;
    size_t clen=0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0)
	{
	  printf("Error in socket creation\n");
	  exit(2);
	}

    printf("Socket retrieve success\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);// adresse du server
    serv_addr.sin_port = htons(5000);
   

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));


    listen(listenfd, 5);
	printf("[+]Sever Listening...\n");

if (argc < 2) 
{
	printf("Enter file name to send: ");
        gets(fname);
}
else
   strcpy(fname,argv[1]);

    while(1)
    {
        clen=sizeof(c_addr);
        printf("Waiting...\n");
        connfd = accept(listenfd, (struct sockaddr*)&c_addr,&clen);
        if(connfd<0)
        {
	  printf("Error in accept\n");
	  continue;	
	}
        // Utilisation de thread
        err = pthread_create(&tid, NULL, &SendFileToClient, &connfd);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
   }
    close(connfd);
    return 0;
}


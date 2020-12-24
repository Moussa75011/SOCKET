// Mise en oeuvre du server
// Ensemble des bibliothèques (Headers) nécessaires pour la mise en oeuvre du programme 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

// Définir un port de communication Client-Server
// de préférence qu'il soit déclaré en variable globale (ne change pas dans le programme)
// comme la valeur pi en trigonomitrie pi = 3.14
#define PORT 4444

// Définir le prototype de la fonction
int factoriel(int num);

int factoriel(int num){
	if(num==0)
		return 1;
	else
		// fonction récursive	
		return num*factoriel(num-1);
	}


int main(){
		
		// Déclaration des variables
		int num1;
		
		// Phase déclarative
		// Déclaration et initialisation de la socket côté Server
		int sockfd;
		// Socket associé au client (elle réalise la liaison d'échange entre le Client et le Server  à
		//partir de la sochet Server)
		int newSocket; // (Socket côté server, structure de la socket du client, taille de la structure)

		// Décrire la structure du client et du server
		struct sockaddr_in serverAddr, newAddr; 

		// Initialisation d'un Buffer
		char buffer[1024];
		
		socklen_t addr_size;
		
		// Phase de Création
		// Créer et construire la socket côté Server "sockfd"
		// On a besoin de 3 arguments
		// AF_INET : Domaine d'@ IPV4 ou IPV6
		// SOCK_STREAM : Configurer en type "flux de données" correspondant au TCP (Retour d'information)
		// "0" : Type de protocole (UDP / TCP) 
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		printf("[+] Server Socket Created Successfully\n");
		
		// Phase de Paramétrage
		// Consiste à définir les différents paramètre de la communication que doit contenir la socket "sockfd"
		// ANYDDR_ANY : Any adress du réseau Server
		// Port de communication "PORT" faire appel à la fonction htons
	
		// Il se trouve des fois qu'il faille initialiser la structure du server (idem pour celle du client)
		// appel d'une fonction "memset" pour juste des précautions à prendre
		memset(&serverAddr, '\0', sizeof(serverAddr));
		
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(PORT); // conversion en octets
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // adresse du server
		
		// Phase de liaison avec le port de communication Client-Server "PORT" Binding
		// Appel de la fonction bind()
		bind(sockfd,(struct sockaddr *) &serverAddr , sizeof(serverAddr));
		printf("[+] Bind to Port Number %d.\n", PORT);

		// Mise en écoute du server par appel de la fonction "listen()"
		// Boucle infinie en attente d'un client
		// L'attente est limité dans le cas du TCP/IP à un maximum de 5 clients
		listen(sockfd, 5);
		printf("\n[+] Server en attente du Client....\n");
		// pour que le server collabore avec le Client, il doit déterminer une nouvelle
		// socket dite de "communication avec le Client" en faisant appel à la fonction "accept()"
		//clilen = sizeof(cli_addr);
		newSocket = accept(sockfd , (struct sockaddr *) &newAddr , &addr_size);
		
		// Si le Server accepte la communication
		// Il va faire appel à la fonction de lecture "read()"
		read(newSocket, buffer, 1024);
		printf("\n (Client) message : %s\n",buffer);
		
		
		printf("\n Réponse du Server: \n");
		
		// Chaine de type pointeur, taille, flot de données pointe vers le fichier
		fgets(buffer, 1024, stdin);
		// Ecrire le message en spécifiant le nombre d'octets dans le buffer "write()"
		write(newSocket, buffer, strlen(buffer));

		// Le server est en attente de la valeur à calculer
		
		read(newSocket, &num1, sizeof(int));					//Read No 1
		printf("(Client) - Fact du nombre is : %d" , num1);
          
		int fact = factoriel(num1);
		
		printf("\nCi-joint votre résultats: ");
		printf("fact = %d \n\n",fact);
		
		// Envoi de la DATA vers le client
		write(newSocket, &fact, sizeof(int));     		//Send No 1 to client

		// Sauvegarder dans un fichier type txt

		FILE *fp;
         int car = 0;
         int mots;
         	// Création de fichier
            fp = fopen("Apple.txt","w");            
            int carac;
			read(newSocket, &mots, sizeof(int));
            
          // Intégrer la valeur calculée dans le fichier créeÉ
          while(car != mots)
       	   {
        	 read(newSocket , buffer , 1024); 
	   	 	fprintf(fp , " %s" , buffer);   
		 	car++;
	  	 }
	   
     	printf("The file was received successfully as Apple_rec\n");
		
		fclose(fp);
		
	
	printf("[+]Ok, I am going to close the connection.\n");
	printf("[+]SEE YOU.\n");
return 0;
}


 
 
/* Server demo */
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE sizeof(struct sockaddr_in)
#define MAX_GET_LEN 256

int main () 
{
	int sockfd, clientsockfd,i=0;
	char c,rc,BNF[MAX_GET_LEN]="";
	struct sockaddr_in server;	

   sockfd = socket(PF_INET, SOCK_STREAM, 0);
   server.sin_family=AF_INET;
   server.sin_port=htons(4321);
   server.sin_addr.s_addr=INADDR_ANY;

   bind(sockfd, (struct sockaddr *)&server, SIZE);

   listen(sockfd,5);
	
	for (;;)	
	{
   	clientsockfd = accept(sockfd, NULL, NULL);	
   	if (fork() == 0)
      {			
      	while ((recv(clientsockfd, &c, 1, 0)>0)&&(i<MAX_GET_LEN)&&c!='\n')	
      	{
         	/*c= toupper(c);*/
				strcat(BNF,&c);
				i++;
				c=toupper(c);
         	send(clientsockfd, &c, 1, 0);	
         }

			send(clientsockfd, BNF, sizeof(BNF), 0);
			c=(char)184;
        	
			/*send(clientsockfd, rc, 1, 0);*/
			send(clientsockfd, &c, 1, 0);		
         close(clientsockfd);
         exit(0);
      }
      close(clientsockfd);	
   }
}

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/*#include <netdb.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>*/
#include <string.h>

#define KEY "12345"
#define MAXGET 256

int main()
{
	struct sockaddr_in client_addr;
	int client_fd,newclient_fd;
	char *line,c;
	int size = 13,i=0;

   client_addr.sin_family=AF_INET;
   client_addr.sin_addr.s_addr=INADDR_ANY;
   client_addr.sin_port=htons(4321);

	client_fd = socket(AF_INET,SOCK_STREAM,0);

   bind(client_fd, (struct sockaddr *)&client_addr,sizeof(client_addr));

   listen(client_fd,5);

	newclient_fd = accept(client_fd,NULL,NULL);

//	while ((recv(client_fd, &c, 1, 0)>0)&&(i<MAXGET)&&c!='\n')	
	while (recv(client_fd, &c, 1, 0)>0)	
	{
   	/*c= toupper(c);*/
		sprintf(line,"%c",c);

		/*c=(char)184;*/
		c= toupper(c);
   	send(client_fd, &c, 1, 0);	
   }

/*	while ((numbytes=recv(newclient_fd,c,1,0))!='\0') buf++;

	recv(newclient_fd,line,size,0);*/

	send(newclient_fd,line,size,0);
	
	printf("%s\n",line);

	exit(0);
}


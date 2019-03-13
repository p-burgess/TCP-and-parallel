/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>
#include <memory.h>

#include <arpa/inet.h>

#define PORT "4321" // the port client will be connecting to

#define MAXDATASIZE 255 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
   if (sa->sa_family == AF_INET) {
       return &(((struct sockaddr_in*)sa)->sin_addr);
   }

   return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
   int sockfd, numbytes;
   char buf[]="this is text";
   struct addrinfo hints, *servinfo, *p;
   int rv;
   char s[INET6_ADDRSTRLEN];

	int n=0;

   if (argc != 3) {
       fprintf(stderr,"usage: client hostname GET-string\n");
       exit(1);
   }

	//fill struct hints with all 0's
   memset(&hints, 0, sizeof hints);	//AF_INET|AF_INET6
   hints.ai_socktype = SOCK_STREAM;	//tcp

	// create linked list of all struct addrinfo * for an ip and port
	/* struct addrinfo {
		int					 ai_flags;		AI_PASSIVE(node=NULL)-can bind|!-can
								 						connect,sendto,sendmsg
											 		AI_CANONNAME-ai_canonname=hostname
													AI_ADDRCONFIG-
		int					 ai_family;		AF_INET,AF_INET6
		int					 ai_socktype;	SOCK_STREAM,SOCK_DGRAM|SOCK_RAW
		int					 ai-protocol;	0=any|IPPROTO_UDP
		size_t				 ai_addrlen;	address length
		struct sockaddr 	*ai_addr;		socket address
		char					*ai_canonname;	hostname if AI_CANONNAME
		struct addrinfo 	*ai_next;

		returns for getaddrinfo(const char *node, const char *service,
										const struct addrinfo *hints,
										struct addrinfo **res);
			res = head of linked list
			0
			EAI_ADDRFAMILY	-	no address in family
			EAI_AGAIN 		-	name server temporary failure
			EAI_BADFLAGS	-	bad flags, name=NULL(AI_CANONNAME)
			EAI_FAIL			-	name server failure
			EAI_FAMILY		-	family not supported
			EAI_MEMORY		-	out of
			EAI_NONAME		-	node or service not known
			EAI_SERVICE		-	no service for socket type
			EAI_SOCKTYPE	-	socket type not supported,inconsistenet
			EAI_SYSTEM		-	errno
	*/
   if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
       return 1;
   }

   // loop through all the results and connect to the first we can
   for(p = servinfo; p != NULL; p = p->ai_next) {
       if ((sockfd = socket(p->ai_family, p->ai_socktype,
               p->ai_protocol)) == -1) {
           perror("client: socket");
           continue;
       }

       if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
           close(sockfd);
           perror("client: connect");
           continue;
       }

       break;
   }

   if (p == NULL) {
       fprintf(stderr, "client: failed to connect\n");
       return 2;
   }

	//translate numeric address to text (s)
   inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
           s, sizeof s);
   printf("client: connecting to %s\n", s);

	//clear linked list
   freeaddrinfo(servinfo); // all done with this structure

	strncpy(buf,argv[2],(sizeof argv[2]+2));
	printf("copied:%s\n",buf);
	
	while ((numbytes=send(sockfd, &buf[n], 1, 0))!='\0') n++;

	/*if (send(sockfd, argv[2], sizeof argv[2], 0) == -1)
				perror("send");*/

   if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
       perror("recv");
       exit(1);
   }

   buf[numbytes] = '\0';

   printf("client: received '%s'\n",buf);

   close(sockfd);

   return 0;
}

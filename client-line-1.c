#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define GET_MAX 255
#define RESP_MAX 255

main ()
{
	struct sockaddr_in serv_addr;
	int server_fd;
	char request[GET_MAX] 	= "Hesdgo World";
	char response[RESP_MAX]	= "";
	int size = 13;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(4321);

	server_fd = socket(PF_INET,SOCK_STREAM,0);
	
	connect(server_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	send(server_fd,request,size,0);

	recv(server_fd,response,size,0);

	printf("%s\n",response);

	recv(server_fd,response,size,0);

	printf("%s\n",response);

	close(server_fd);

	exit(0);
}

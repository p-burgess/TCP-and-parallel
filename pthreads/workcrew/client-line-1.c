#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define GET_MAX (255)

int code_buf(char in_buf[],char out_buf[]);

main ()
{
	struct sockaddr_in serv_addr;
	int server_fd;
	char request[GET_MAX];
	char raw_response[GET_MAX]	= "";
	char decode_response[GET_MAX] = "";
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(4321);

	server_fd = socket(PF_INET,SOCK_STREAM,0);

	//enter GET string
	printf("Enter the GET string: ");
	fgets(request,GET_MAX-1,stdin);
		
	connect(server_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	
	send(server_fd,request,GET_MAX,0);
	
	printf("client has sent:%s\n",request);
	
	sleep (2);

	recv(server_fd,raw_response,GET_MAX,0);

	code_buf(raw_response,decode_response);
	
	printf("client has received:%s\n",decode_response);

	close(server_fd);

	exit(0);
}

int code_buf(char in_buf[GET_MAX ],char out_buf[GET_MAX ])
{
	int x;
	
	for(x=0;x<GET_MAX; x++)
	{
		out_buf[x]=in_buf[x]^4;
	}
			
	return 0;
}

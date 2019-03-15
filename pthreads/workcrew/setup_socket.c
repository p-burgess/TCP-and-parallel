#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <sys/ioctl.h>

#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

//#include "arg.h"

int setup_sock (void)
{
	struct sockaddr_in client_addr;
	memset (&client_addr,0,sizeof(client_addr));

	int sock;
	int on = 1;

	client_addr.sin_family=AF_INET;
	client_addr.sin_addr.s_addr= htonl (INADDR_ANY);
	client_addr.sin_port=htons(4321);

	sock = socket (PF_INET,SOCK_STREAM,0);	
	setsockopt (sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof on);
//	fprintf (stderr,"socket options set\n");

	ioctl (sock,FIONBIO,(char *)&on);
	bind (sock, (struct sockaddr *)&client_addr,sizeof (client_addr));

	listen (sock,5);
//	fprintf (stderr,"listening\n");
	
	return sock;
}
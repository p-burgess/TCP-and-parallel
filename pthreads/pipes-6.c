#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>

#define MSGSIZE 256
#define GET_MAX 256

// void parent_process(int fifoname,n);
size_t readLine(int fd,void *vptr,size_t maxlen);
size_t writeLine(int fd,const void *vptr, size_t n);

int main() 
{
	int n_grandchild;		// number of grandchilds
	int gp[2],p[2]; 		// pipes
	int f,i;
	char message[256],inbuf[256];
	pid_t pid;

	// number of  grandchildren
	n_grandchild=1;

	// create 
	if (pipe(gp) == -1) {
		exit(1);
	}

	// create child
	switch(pid=fork()) {
	case -1: 
		   exit(2);
	case 0:
		if (pipe(p) == -1) {
			perror("parent pipe failed");
		}

		struct sockaddr_in client_addr;
		int client_fd,newclient_fd,c_fd;
		char in_request[GET_MAX];
		char out_response[GET_MAX];
	
		client_addr.sin_family=AF_INET;
		client_addr.sin_addr.s_addr=INADDR_ANY;
		client_addr.sin_port=htons(4321);

		// create socket
		client_fd = socket(AF_INET,SOCK_STREAM,0);
		bind(client_fd, (struct sockaddr *)&client_addr,sizeof(client_addr));

		// create message
		//sprintf( message,"child pid:%d",getpid() );
		// send message to parent process
		//writeLine( p[1],message,sizeof(message) );

		// create grandchilds
		for (i = 0; i < n_grandchild; i++)
			switch(fork()){
			case -1:
				perror("fork failed");
				break;
			case 0:

				// child
				// create message
				//sprintf( message,"grandchild pid:%d",getpid() );
				readLine( gp[0],message,sizeof(message) );
				sscanf(message,"%d",&c_fd);
				printf("file descriptor from parent: %s",message);
				strcpy(message,"this is the return message");
				writeLine( c_fd,message,sizeof(message) );
				printf("server has sent: %s",message);

				break;
			default:
				exit(0);
				break;
		}
		
		// get connnection
		listen(client_fd,5);
		newclient_fd = accept(client_fd,NULL,NULL);

		// send connection message to child
		sprintf(message,"%d",newclient_fd);
		writeLine( gp[1],message,sizeof(message) );

		// get message from p[]arent pipe
		//readLine( p[0],inbuf,sizeof(inbuf) );
		///printf("%s\n", inbuf);
		// send message on gp[]arent pipe
		//sprintf( message,"from child:%s",inbuf );
		//writeLine( gp[1],message,sizeof(message) );

	default:
		// read message from all childs on the pipe
	   /*for(f=0;f<(n_grandchild+1);f++) {
			readLine( gp[0],inbuf,sizeof(inbuf) );
	      printf("%s\n", inbuf);
	   }*/
		wait(NULL);
	}
	exit(0);
}

size_t writeLine(int fd, const void *vptr, size_t n) 
{
	size_t		nleft;
	ssize_t		nwritten;
	const char 	*buffer;

	buffer=vptr;
	nleft=n;

	while( nleft>0 ){
		if( (nwritten=write(fd,buffer,nleft) )<=0 ) {
			if( errno==EINTR )
				nwritten=0;
			else
				return -1;
		}
		nleft-=nwritten;
		buffer+=nwritten;
	}

	return n;
}

size_t readLine(int fd,void *vptr,size_t maxlen) 
{
	ssize_t n,rc;
	char c,*buffer;

	buffer=vptr;

	for( n=1;n<maxlen;n++ ) {

		if( (rc=read(fd,&c,1))==1 ) {
			*buffer++ =c;
			if( c=='\n' )
				break;
		}
		else if( rc==0 ) {
			if( n==1 )
				return 0;
			else
				break;
		}
		else {
			if( errno==EINTR )
				continue;
			return -1;
		}
	}
	*buffer=0;
	return n;
}


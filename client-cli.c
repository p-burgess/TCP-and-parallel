 /* Client demo */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/times.h>

#define SIZE sizeof(struct sockaddr_in)

int getch_s();
int cs_error();

int main (int argc, char **argv)
{
	int sockfd;
	char c,rc;
	struct sockaddr_in toserver;
  
  


/* Test command line arguements */
	if (argc==1){
		cs_error("Incorrect Usage");
		exit(2);
	}
	
	toserver.sin_addr.s_addr = inet_addr(argv[1]);
   toserver.sin_port=htons(atoi(argv[2]));
	toserver.sin_family=AF_INET;

   sockfd = socket(PF_INET, SOCK_STREAM, 0);
   connect(sockfd, (struct sockaddr *)&toserver, SIZE);

	for (rc='\n';;)
   {
      if (rc == '\n')	
         printf("Enter a lowercase char");		
      c= getch_s();
      send(sockfd, &c, 1, 0);
      if (recv(sockfd, &rc, 1, 0)>0)
         printf("%c\n",rc);
      else
         exit(1);
   }
}

int cs_error(const char *msg)
{
	clock_t currentTime;
	/* print to log file with time. */
	printf("mt:%s:%f ",msg, currentTime);

	/* print error to console */
	printf(" Usage: client IP-server port GET-string");
}

int getch_s()
{
	int ch;
	struct termios oldt;
	struct termios newt;

	tcgetattr(STDIN_FILENO, &oldt); /*store old settings*/
	newt = oldt; /* copy old settings to new settings */
	newt.c_lflag &= ~(ICANON | ECHO); /* change settings */
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
	ch = getchar(); /* standard getchar call */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /* reapply the old settings */
	return ch; /* return received char */
}

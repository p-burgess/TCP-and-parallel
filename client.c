 /* Client demo */
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE sizeof(struct sockaddr_in)
int getch();
int main () {

int sockfd;
char c,rc;
struct sockaddr_in toserver;
   toserver.sin_family=AF_INET;
   toserver.sin_addr.s_addr = inet_addr("127.0.0.1");
   toserver.sin_port=htons(4321);
   sockfd = socket(PF_INET, SOCK_STREAM, 0);
   connect(sockfd, (struct sockaddr *)&toserver, SIZE);

/* Poll user for connection detatils */


for (rc='\n';;)
   {
      if (rc == '\n')	
         printf("Enter a lowercase char");		
      c= getch();
      send(sockfd, &c, 1, 0);
      if (recv(sockfd, &rc, 1, 0)>0)
         printf("%c\n",rc);
      else
         exit(1);
   }
}

int getch()
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

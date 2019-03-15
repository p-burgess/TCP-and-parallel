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
#include <time.h>
#include <sys/times.h>
#include <string.h>

#define SIZE sizeof(struct sockaddr_in)

int getch_s();
int cs_message(FILE *logfd, const char *msg);

int main (int argc, char *argv[])
{
	int sockfd;
	const char *a="a";	/* file permission */
	char rc;
	char *message="",*BNF="";
	char c;
	struct sockaddr_in toserver;

	/* open log file for ammendment */
	FILE *logfd;
	logfd = fopen("./cs.log", a);
	printf("%d\n",ferror(logfd));

/* Test command line arguements */
	/*if (argc==1){
		cs_message(logfd, "1:m:t:incorrect usage");
		exit(2);
	}*/

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
      send(sockfd, &argv[3], sizeof(argv[3]), 0);
      while (recv(sockfd, &rc, 1, 0)>0){
			if (rc==(char)184){
				sprintf(message, "2:m:s:p:t:stop signal from:%s:%s", argv[1],argv[2]);
				/*printf("%c",rc);}*/
				cs_message(logfd, message);
				exit(3);
			}
			strcat(BNF,&rc);
      }
		printf("%s\n", BNF);
		fclose(logfd);
		exit(1);
   }
}

int cs_message(FILE *logfd, const char msg[])
{
	time_t currentTime;
	struct tm* tm;

	currentTime=time(0);
	tm=localtime(&currentTime);

	/* print to log file with time. */
	fprintf(logfd, "%s:%d:%d:%d:%d:%d\n",msg, tm->tm_year+1900, tm->tm_yday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	fflush(logfd);

	/* print error to console */
	if (msg[0]=='1')printf(" Usage: client IP-server port GET-string\n");
	else if (msg[0]=='2')printf(" Stop signal received\n");
	else printf("%s",msg);
	return (0);
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

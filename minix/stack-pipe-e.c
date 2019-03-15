#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h> 

#define READ_END 0
#define WRITE_END 1

#define BUFSIZE 63

int n = 3;
int total_sys_time = 0;
int total_usr_time = 0;

void catch_sigchild(int signo);

int main (int argc, char **argv) 
{
	pid_t pid;
	int fd[][2];
	int i, tot = 0;
	char *buffer_in;
	char *buffer_out;
	struct sigaction act;
	
	newact.sa_handler = catch_sigchild;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if sigaction(SIGCHLD, &act, NULL) == -1)
		perror("Could not install signal handler");
	
	pipe(fd);

	for (i=0; i < 3; i++) {
		pipe(fd[i]);
		fork();
			if (pid==0) {
				close(fd[i][READ_END]); /*close read end since I don't need it */
				dup2(fd[i][WRITE_END], STDOUT_FILENO);
	
		/* create some numbers to send to parent */
		
			
			printf(":%d:%d:%d:", childpid, user_time, sys_time);
				
				close(fd[i][WRITE_END]);
				exit(0);
		//break; /*exit for loop to end child's code */
			} else {
		
				close(fd[i][WRITE_END]);
		//do parenty things
		//dup2(fd[READ_END], STDIN_FILENO);
		//scanf("%d",

			}
	}
	
	/* 	use select to read from pipe */
	
	/* signal handler to count the number of children that exit 
	 * when all processes finished, calculate averages and
	 * print to user. 
	 * 
	 * catch signal
	 * decrement number_of_processes
	 * if number_of_processes = 0
	 *		calculate average
	 *		print to user
	 */
	 
	int num_fds;
   int i;
   int num_ready;
   int num_now;
   int bytes_read;
   char buf[BUFSIZE];
   struct pollfd *myfd;
   short errormask;
  
   errormask = POLLERR|POLLHUP;
 
                                 /* initialize the polling structure */
   if ((myfd = (void *)calloc(num_fds, sizeof(struct pollfd))) == NULL)
      return;
   for (i = 0; i < num_fds; i++) {
      (myfd + i)->fd = *(fd + i);
      (myfd + i)->events = POLLRDNORM;
      (myfd + i)->revents = 0;
   }

   num_now = num_fds;
 
        /* Continue monitoring until all I/O on all descriptors done */
   while (num_now > 0) {
      if ( ((num_ready = poll(myfd, num_fds, INFTIM)) == -1) &&
           (errno != EINTR) )
          break;
 
      for (i = 0; i < num_fds && num_ready > 0; i++)  {
          if ( (myfd + i)->events && (myfd + i)->revents ) {
                   /* if an error occurred, do not monitor fd anymore */
             if ((myfd + i)->revents & errormask) {
                num_now--;
                (myfd + i)->events = 0;
             }
                          /* if a normal data arrival, read the data */
             else if ((myfd + i)->revents & POLLRDNORM) {
                bytes_read = read(fd[i], buf, BUFSIZE);
                num_ready--;
                if (bytes_read > 0){
				                  	
					buffer_out = strtok(buffer_in,":");
					fflush(buffer_in);
					printf("%s\n", buffer_out);
					
				} else {
                   num_now--;
                   (myfd + i)->events = 0;
                }
             }   
          }   
      }    
      if (!num_now) break;
   }


	

	tot	+= atoi(buffer_out);
	//printf("%d\n",tot);
	
    close(fd[READ_END]);
    close(fd[WRITE_END]);
    
    wait(NULL); 
	
	//printf("%d\n", tot);
	/*while (1){
        wait(NULL);
        if(errno== ECHILD) {
            printf("all children ended\n"); 
            break;
        }*/
 

return 0;
}

void catch_sigchild(int signo)
{	
	int ast, aut;
	int a;
	
	a = n;
	if (--a == 0)
	{
		ast = total_sys_time / n;
		aut = total_usr_time / n;
		printf("ast:\t%d\taut:\t%d", ast, aut);
	}
}
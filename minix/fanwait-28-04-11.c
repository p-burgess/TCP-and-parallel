#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>

#define BUFSIZE 63

/* prototypes */
int child_process(char *);
int parent_process(char *, int);

int main(int argc, char *argv[]) {
	/* pid_t childpid; */
	
	char *fifoname="fifo_pipe";
	int i, n;
		
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [number_processes] \n", argv[0]);
		return 1;
	}
	
	/* make a FIFO pipe for ipc */
	if (mkfifo(fifoname, S_IRUSR | S_IWUSR) == -1) {           /* create a named pipe */
		if (errno != EEXIST) {
			perror("failed to create named pipe\n");
			return 1; 
		}
	}
		

	
	n = atoi(argv[1]);
	for (i = 1; i < n; i++)
	switch(fork()){
	case -1:
		perror("fork failed");
		break;
	case 0:
		/* execute a file (argv[2]) */
		child_process(fifoname);
		break;
	default:
		parent_process(fifoname,n);
		break;
	}
	exit(0);
}

int parent_process(char *fifoname, int number_process)
{
	char *buf[BUFSIZE];
	int rval;
	int fifo_parent;
	int child_pid, usr_time, sys_time;
	int avg_sys_time, avg_usr_time, total_sys_time, total_usr_time;
	int i = 0;
	
	/* open fifo for reading */
	while (((fifo_parent = open(fifoname, O_RDONLY)) == -1) && (errno == EINTR)){	; 
		if (fifo_parent == -1) {
			fprintf(stderr, "[%ld]:failed to open named pipe %s for read: %s\n",
			(long)getpid(), fifoname, strerror(errno));    
			return 1; 
		}
	}	
	
	
	/* wait for all processes to finish 
	for (i = 0; i<number_processes; i++)
		wait(NULL);*/
	
	/* read from pipe */
	for (i = 0; i<number_process; i++){
		rval = read(fifo_parent, buf, BUFSIZE);	
		if (rval == -1) {
			fprintf(stderr, "[%ld]:failed to read from pipe: %s\n",
			(long)getpid(), strerror(errno));    
			return 1; 
		}

	}
	
	/*while(wait(NULL) > 0 && (errno == EINTR)) ;    
		fprintf(stderr, "i:%d  process ID:%ld  parent ID:%ld  child ID:\n",
		   i, (long)getpid(), (long)getppid());*/
		   
		printf("child:\t%s\n", buf);
		
		/*write to message STDOUT */
			

		//printf("child:\t%s\n", buf);
		
		/*child_pid = atoi(strtok(buf,":"));
		usr_time = atoi(strtok(buf,":"));
		sys_time = atoi(strtok(buf,":"));
		
		total_sys_time += sys_time;
		total_usr_time += usr_time;
		
		i++;
		strtok(NULL,":");*/
		
		/* colate numbers */
		
		/* wait for all children 	
		 */

	
	/* calculate and print data results */
	avg_sys_time = total_sys_time/i;
	avg_usr_time = total_usr_time/i;
	printf("Totals:\nsystem time:\t%d\tuser time:\t%d\n", total_sys_time, total_usr_time);
	printf("\nAverages:\nsytem time:\t%d\tuser time:\t%d\n",avg_sys_time, avg_usr_time);
	
	return 0;
}

int child_process (char *fifoname)
{
	 /* child variables */
	int mat1[300][300], mat2[300][300];
    int result[300][300];
    int n =300;
	
	int i, j, k;
	char buf[BUFSIZE];
	int fifo_child, sval;
    ssize_t strsize;
	
	clock_t tcstart, tcend;
	struct tms start, end;
	
	while (((fifo_child = open(fifoname, O_WRONLY|O_NONBLOCK)) == -1) && (errno == EINTR)) ; 
		if (fifo_child == -1) {
			fprintf(stderr, "[%ld]:failed to open named pipe %s for write: %s\n", 
			(long)getpid(), fifoname, strerror(errno));
			return 1; 
		} 
	
	/* execute program */
		
	/* start timer */
	if ((tcstart=times(&start))==-1){
		printf("Error to get start time\n");
		return 1;
	}
		
	/* initialize matricies */
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
		   mat1[i][j] = rand();
		}
	}
		
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			mat2[i][j]= rand();
		}
	}
		
	/* now multiply matricies... */
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			result[i][j] = 0;
			for (k = 0; k < n; k++)
				result[i][j] += mat1[i][k] * mat2[k][j];
		}
	}

	/* end timer and print */
	if ((tcend=times(&end))==-1){
		printf("Error to get final time\n");
		return 1;
	}

	/* make string to send on pipe */
	sval = snprintf(buf, BUFSIZE, "%ld:%d:%d:\n",(long)getpid(), (int)(end.tms_utime-start.tms_utime),(int)(end.tms_stime-start.tms_stime));
	if (sval < 0) {
		fprintf(stderr, "[%ld]:failed to make the string:\n", (long)getpid());
		return 1; 
	} 

	/* wait for signal to write to pipe */
	
	
	/* write to fifo pipe */
	strsize = strlen(buf) + 1;
	sval = write(fifo_child, buf, strsize);
	if (sval != strsize) {
		fprintf(stderr, "[%ld]:failed to write to pipe: %s\n",
		(long)getpid(), strerror(errno));
		return 1;
	}
	
	/* close the pipe */
	close(fifo_child);
	
	return 0;
}

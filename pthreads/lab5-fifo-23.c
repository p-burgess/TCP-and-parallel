/****************
 Example program to demonstrate use of FIFOs (named pipes)
 ****************/
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include <errno.h>
#include <string.h>



typedef struct log_arg {
	FILE *log_write;
	FILE *fifo_read;
} log_arg;

void *logger(void *log_arg);
 
int main()
{
//	int ret;
//	pid_t pid;
//	int value;
	char *message;//,inbuf[256];

	message = malloc (256 * sizeof(char));
	message = "this is a message";
	

/* access check the log files, enforce a naming convention */

	sleep(1);

	FILE *fifo_write;

	char *fifoName="./testfifo.txt";

	/* 0600 -read, write to user, and none to group and world */
	if(mknod(fifoName, S_IFIFO | 0666, 0) < 0){
		fprintf(stderr,"Unable to create fifo: %s",fifoName);
		exit(1);
	}

	if((fifo_write = fopen(fifoName,"w")) == NULL) 
	{
		fprintf(stderr,"Unable to open fifo for writing");
		unlink(fifoName);
	}

	logger((void *)fifoName);	

	fprintf (fifo_write,"hello logger\n");
	sleep (1);

	fflush(fifo_write);
	fclose(fifo_write);
   unlink(fifoName); /* Delete the created fifo */
	exit(0);
}



void *logger(void *log_arg)
{
	time_t currentTime;
	struct tm * Time;
	char *inbuf;
	char *fifoName;

	fifoName=log_arg;
//	fifoName = (char *) malloc (13 * sizeof(char)); 
//	sprintf(fifoName,"./testfifo.txt");

	FILE *fifo_read;
	FILE *log_write;

	inbuf = (char *) malloc (256 * sizeof(char));

//	arg = log_arg;
sleep(1);
	// record the time
	currentTime=time (NULL);
	Time = localtime ( &currentTime );

	// open FIFO for reading
   if ((fifo_read = fopen(fifoName,"r")) == NULL)
		fprintf(stderr,"Unable to open fifo for reading");

	// open log file for appending
   if ((log_write = fopen ("./log.text","w")) == NULL)	;
		fprintf(stderr,"Unable to open log file appending");

	// write log entry

	/* timestamp log messages */
	fprintf (log_write,":%d:",Time->tm_year+1900);
	fprintf (log_write,"%d:",Time->tm_mon+1);
	fprintf (log_write,"%d:",Time->tm_mday+1);
	fprintf (log_write,"%d:",Time->tm_hour);
	fprintf (log_write,"%d:",Time->tm_min);
	fprintf (log_write,"%d:",Time->tm_sec);
	fprintf (log_write,"logger started\n");

	sleep(1);

	fread( inbuf,256,1,fifo_read );


	fprintf (log_write,"\n");

//      fclose(log_read);
//      fclose(log_write);
   exit (0);

 }


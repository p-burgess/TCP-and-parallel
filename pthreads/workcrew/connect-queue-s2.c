#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ctype.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>
#include <string.h>

#define GET_MAX 256
typedef struct connect connect_t;

struct connect {
	/* Link field for linked list. */
	connect_t *next;
	connect_t *prev;

	/* Other feilds describing work to be done. */
	char *msg;
	char *resp;
	char **words;
	int client_fd;
	struct tm * time;	
};

typedef struct connect_queue
{
	connect_t *first;
	connect_t *last;
} connect_queue_t;

connect_queue_t *connect_queue;

typedef struct arg arg_t;

struct arg {
	int fd;//FILE *fd;
	FILE *log_fd;
};

void* servlet (void* arg_t);
void* connect_accept (void* arg_t);
void* queue_connection (int client_fd,void *buffer);
void* respond (connect_t *client);
int code_buf(char *in_buf,char *out_buf);
void mysighup ();

/* A linked list of pending jobs. */
/* The queue is intially empty */
//struct job *connect_head;
//struct job *connect_end;

/* A mutex protecting job_queue. */
pthread_mutex_t connect_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* A condidition indicating connections in the queue. */
pthread_cond_t  not_empty = PTHREAD_COND_INITIALIZER;

int sharedData=0;
int queue_predicate=0;

int main ()
{
	//	connect *fifo;
	pthread_t serv, gate;

	arg_t *arg;
	FILE *log;

	if (signal(SIGHUP, mysighup) == SIG_ERR)
		printf("no handle for SIGHUP");
	
	if ((arg = (arg_t *) malloc (sizeof (arg_t))) == NULL) {
		fprintf (stderr,"Unable to malloc arg");
		exit(0);
	}

	if ((log = (FILE *) malloc (sizeof (FILE))) == (FILE *)NULL) {
		fprintf (stderr,"Unable to malloc arg->fd");
		exit(0);
	}

	connect_queue = (connect_queue_t *) calloc (1,sizeof (connect_queue_t));
	connect_queue->first = connect_queue->last = 0;
	
	/*connect_head = (struct connect *)NULL;
	connect_end = (struct connect *)NULL;*/
	
//	pthread_mutex_init(&connect_queue_mutex, NULL);
	
	/* Initialize the condition variable for 
	the queue. Its intitial value should be zero. */
//	pthread_cond_init (&not_empty,NULL);
	
	fprintf (stderr,"\nqueue initialized: %d\n",(int)connect_queue->first);

	log = fopen ("./log.text", "a");
	arg->log_fd = log;
	fprintf (log,"program started");
		
	pthread_create (&serv, NULL, servlet, arg);

	arg->fd = 0;
	pthread_create (&gate, NULL, connect_accept, arg);

	pthread_join (serv, NULL);
	pthread_join (gate, NULL);
	
	pthread_mutex_destroy (&connect_queue_mutex);
	pthread_cond_destroy (&not_empty);
	
	close (arg->fd);
	return 0;
}

/* Add a new job to the front of the job queue. */
void *queue_connection (int newclient_fd,void *buffer/* Pass job-specific data here... */)
{
	connect_t *ptr;
	time_t currentTime;
	
	int rc = 0;
	int amountOfData = 4;
	int i;

//	int j;
	
//	newclient_fd;

	printf ("queue connection started\n");

	ptr = (connect_t *) calloc (1,sizeof (connect_t));
	ptr->client_fd = (int) calloc (1,sizeof (int));
	ptr->time = (struct tm *) malloc (sizeof (struct tm *));
//	ptr->msg = (char *) calloc (1,GET_MAX * sizeof (char));

//	j = recv(newclient_fd,in_request,GET_MAX,0);

	if (ptr == NULL)							/* error allocating node?      */
		return (struct node *) NULL;		/* then return NULL, else      */
		else {									/* allocated node successfully */	
		ptr->time = localtime (&currentTime);
		ptr->msg = (char *)buffer;			/* fill in message details     */
		ptr->client_fd = newclient_fd;	/* copy id details             */
	}
	
	printf ("server recieved: %s",ptr->msg);

	while (amountOfData--) {
		/* Lock the mutex on the connect queue before accessing it. */
		rc = pthread_mutex_lock (&connect_queue_mutex);

		printf ("mutex locked\n");

		/* inserts a node at the end of the list. */
		if (connect_queue->last) {    /* if an empty list,          */
			connect_queue->last->next = ptr;        /* join the two final links together. */
			ptr->prev = connect_queue->last;
			connect_queue->last = ptr;
		} else {
			connect_queue->first = ptr;
			connect_queue->last = ptr;
		}	
		//	connect_head->next = NULL; /* set end of list to NULL    */
		//    return;                 /* and finish                 */

		//ptr->next = connect_head;             /* link next field to original list   */
		//connect_head = ptr;                   /* head adjusted to new node          */

		/* Signal waiting threads that a new job has been 
		added to the connect queue. */
		++sharedData;
		queue_predicate = 1;
		rc = pthread_cond_broadcast (&not_empty);
		if (rc) {
			pthread_mutex_unlock (&connect_queue_mutex);
			printf ("Producer: Failed to wake up consumer, rc=%d\n",rc);
			exit (1);
		}
		/* Unlock the connect queue mutex. */
		printf ("Producer: Unlock shared data and flag\n");
		rc = pthread_mutex_unlock (&connect_queue_mutex);

		fprintf (stderr,"mutex released\n");
		
		sleep (2);
		return NULL;
	}
}
  
/* Process queued jobs until the queue is empty. */
void* connect_accept (void *arg)
{
/*	connect_t next_connect;
	
	//FILE *int log_write;
	char buffer[256];*/

	struct sockaddr_in client_addr;
	struct timeval timeout;
	
	fd_set master_set,read_fd;

	int sock,max_sd;
	int desc_ready, end_server = 0;
	int close_conn;
	void *buffer;

	int i,j,rc,on=1;
	//int so_reuse = 1;
	//size_t size;

	arg_t *args;
	
	args = arg;
	
	memset (&client_addr,0,sizeof(client_addr));
	client_addr.sin_family=AF_INET;
	client_addr.sin_addr.s_addr= htonl (INADDR_ANY);
	client_addr.sin_port=htons(4321);

	sock = socket (PF_INET,SOCK_STREAM,0);	
	setsockopt (sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof on);
	fprintf (stderr,"socket options set\n");

	ioctl (sock,FIONBIO,(char *)&on);
	bind (sock, (struct sockaddr *)&client_addr,sizeof (client_addr));

	args->fd = sock;
	
	listen (sock,1);
	fprintf (stderr,"listening\n");

	FD_ZERO (&master_set);
	max_sd = sock;
	FD_SET (sock,&master_set);
	//	fprintf (stderr,"FD_ZERO set\n");

	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	/* Loop for incoming connections */
	do {
		memcpy (&read_fd,&master_set, sizeof (master_set));
		rc = select (max_sd+1,&read_fd,NULL,NULL,&timeout);
		if (rc < 0)
			break;
		if (rc == 0)
			break;
		desc_ready = rc;
		for (i=0;i<=max_sd && desc_ready > 0;++i) {
			if (FD_ISSET (i,&read_fd)) {
				desc_ready -= 1;
				if (i == sock) {
					do {
						fprintf (stderr,"connection accepted\n");
						j = accept (args->fd,NULL,NULL);
						if (j < 0) {
							if (errno != EWOULDBLOCK) 
								end_server = 1;
							break;
						}
						FD_SET (j,&master_set);
						if (j > max_sd)
							max_sd = j;
					} while (j != -1);
				} else {
					close_conn = 0;
					do {
						buffer = calloc (1,256 * sizeof (char));
						rc = recv(i,buffer,256 * sizeof (char),0);
						if (rc < 0) {
							if (errno != EWOULDBLOCK) {
								close_conn = 1;
							}
							break;
						}
						if (rc == 0) {
							close_conn = 1;
							break;
						}
						queue_connection (i,buffer);
					} while (1);
					if (close_conn) {
						close (i);
						FD_CLR (i,&master_set);
						if (i == max_sd) {
							while (FD_ISSET (max_sd,&master_set) == 0)
								max_sd -= 1;
						}
					}
				}
			}
		}
	} while (end_server == 0);

	for (i=0;i<=max_sd;i++) {
		if (FD_ISSET (i,&master_set))
			close (i);
	}
	return NULL;
}


void *servlet (void *arg)
{
	/*remove connection from the end of the connect list and
	respond appropriately. */
	int j;
	int rc;
	int retries=2;

	arg_t *args;
	connect_t *next_connect;//,*temp;

	struct tm * Time;	
	time_t currentTime;

	char *buffer;

	args = arg;
	
	buffer = (char *) malloc (256 * sizeof (char));
	
	// create timestamp
	currentTime=time (NULL);
	Time = localtime ( &currentTime );

	j = sprintf (buffer,":%s\n",asctime (localtime (&currentTime)));
	j += sprintf (buffer+j,"servlet started\n");
	// write log entry
	fwrite ((void *)buffer,sizeof (buffer),1,args->log_fd);


	printf ("start mutex lock\n");
	/* Lock the mutex on the connect queue. */
	pthread_mutex_lock (&connect_queue_mutex);

	while (retries--) {
		/* Wait on the connect queue condition. If
		the queue is empty, block until a new job 
		is enqueued. */
		
		while (queue_predicate==0) {
			printf ("queue empty\n");
			if (pthread_cond_wait (&not_empty,&connect_queue_mutex)) {
				pthread_mutex_unlock (&connect_queue_mutex);
				exit (1);
			}
		}
	
		printf("queue condition not empty");
		/* Because of the condition, we know the queue is not
		empty. Get the next available job. */
	//		next_connect = connect_end;

		next_connect = connect_queue->first;
		connect_queue->first = next_connect->prev;
		connect_queue->first->prev = connect_queue->last;

		--sharedData;
		if (sharedData == 0) {queue_predicate = 0;}
	}
/* Unlock the mutex on the job queue because we're
done with the queue for now. */

/*		lastif (next_job == NULL) {
		printf("end of list NULL");
		continue;
	}
*/
	//    struct node *temp;
	//  temp = ptr;    /* node to be deleted */
	//  temp = head;   /* start of the list, will cycle to node before temp    */
						/* we deleting first node  */
	
	/* adjust the end of the list to point to
	second last node. */
/*		temp = connect_head;
	while (temp != end){
		temp = temp->next;
	}
	connect_end = temp;                  moves head to next node     */
	
		//free( temp );                   /* free space occupied by node */


	/* Carry out the work. */
	//		process_job (next_job);
	/* timestamp log messages */
	// create timestamp
	currentTime=time (NULL);
	next_connect->time = localtime (&currentTime);

	respond (next_connect);
	printf("response sent");
	pthread_mutex_unlock (&connect_queue_mutex);
	
	// create log entry.
	buffer = "";
	j = sprintf (buffer,":%s\n",asctime (localtime (&currentTime)));
	j += sprintf (buffer+j,"client request:%s:",next_connect->msg);

	// write log entry
	fwrite ((void *)buffer,sizeof (buffer),1,args->log_fd);
	
	// Clean up
	free (next_connect);

	j = 0;
	
	fprintf (stderr,"response sent");


	return NULL;
}

void *respond (connect_t *a)
{
	char *response = "response";
	char *code_resp;
	
	code_resp = (char *) malloc (sizeof (GET_MAX));
	
	code_buf (response,code_resp);
	
	a->resp = (char *) calloc (1,sizeof (response));
	strcpy(a->resp,code_resp);
	
	send(a->client_fd,a->resp,sizeof (a->resp),0);
	
	fprintf (stderr,"response processed");
	
	return NULL;
}

int code_buf(char *in_buf,char *out_buf)
{
	int x;
	
	for(x=0;x<GET_MAX; x++)
	{
		out_buf[x]=in_buf[x]^4;
	}
			
	return 0;
}

void mysighup ()
{
		printf("caught SIGHUP\n");
	// kill all threads
	// close all file descriptors
	// destroy all mutex
}

/*adapted from stackoverflow.com/questions/2641101/*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string.h>

#define MAX_LENGTH 40

int log_fd;

typedef struct read_userID_arg {
	char *filename;
	struct userID *head;
} read_userID_arg;

typedef struct job {

	struct job* next;

/* Other fields describing work to be done... */
	char *message;
} job;

typedef struct userID {
	int ukey;
	char *uname;
	struct userID *next;
} userID;

struct userID *head = (struct userID *) NULL;
struct userID *end = (struct userID *) NULL;

/* A linked list of pending jobs.*/
struct job* job_queue;

/* A mutex protecting job_queue. */
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

struct userID * init_userID (char *uname, int ukey);
void append_userID (userID *anchor,userID *new);
void delete_userIDs (userID *first);
void print_userIDs (userID *head);

void *read_conf(void *r_arg);

int main ()
{
	char *filename = "data.txt";
	pthread_t read_userID;
	read_userID_arg	*arg;
	int thread_flag;


	arg = (void *) malloc (sizeof (read_userID_arg));
	arg->head = (userID *) malloc (sizeof (userID));
	arg->filename = (char *) malloc (sizeof (filename));

	strcpy(arg->filename,filename);
	arg->head = head;

//	printf(",%s,",arg->filename);

	/* populate the user database. */
	read_userID = (pthread_t) malloc (sizeof (pthread_t));
	pthread_create (&read_userID, NULL, read_conf, arg);
	pthread_join (read_userID, NULL);
	
	/* Set up the logger. */


//	print all userIDs
	print_userIDs (head);

//	delete all userIDs
	delete_userIDs (head);

	exit (0);
}

//struct userID *
void *read_conf (void *r_arg)//,struct userID *head)
{
	FILE *my_file;
	char *conf_file;
	userID *ptr,*new;
	read_userID_arg *arg;

	arg = r_arg;

	new = (userID *) malloc (sizeof (userID *));
	head = (userID *) malloc (sizeof (userID *));
	end = (userID *) malloc (sizeof (userID *));

	head->next=new;
	new=head;
	ptr=head;

	conf_file = malloc (strlen(arg->filename));

	if (conf_file==NULL) 
		return (void *)EXIT_FAILURE;
	strcpy (conf_file,arg->filename);

	char text_read[MAX_LENGTH];
	char uname_text[64];
	char ukey_text[64];
	int value;

	if ((my_file = fopen (conf_file,"r")) == NULL)
	{
		fprintf(stderr, "Error opening data file:  data.txt");
		return (void *)EXIT_FAILURE;
	}

	free(conf_file);

	while (fgets(text_read, MAX_LENGTH, my_file)!=NULL)
	{
		char * p;
		//----------------------------------------------
		//  Find the separator.
		//----------------------------------------------
		p = strchr(text_read,':');

		uname_text[0] = '\0';
		ukey_text[0] = '\0';

		if (p != 0 && text_read != NULL)
		{
			size_t  uname_length = 0;
			uname_length = p - text_read;

			//  Skip over the separator
			++p;
			strncpy(ukey_text,p,uname_length-1);
			value = atoi(ukey_text);

			strncpy(uname_text, text_read, uname_length);
			uname_text[uname_length] = '\0';

			ptr=init_userID (uname_text,value);

			append_userID (new,ptr);

			new=ptr;
			new->next=end;

//			printf (",%s,%d,\n",ptr->uname,ptr->ukey);

	  } else {
		   fprintf(stdout,"EOF %s\n",text_read);
	  }
	} // End:  while fgets

	fclose (my_file);
	free (new);
//	free (head);

	pthread_exit(0);

	return (void *)EXIT_SUCCESS;
}

userID * init_userID (char *name, int key)
{
	struct userID *ptr;
	ptr = (struct userID *) malloc (sizeof (struct userID));
	ptr->uname = (char *) malloc (sizeof (name));
	ptr->ukey = (int) malloc (sizeof (int));

	if (ptr==NULL)
		return (struct userID *) NULL;
	else {
		strcpy(ptr->uname,name);
		ptr->ukey=key;
		ptr->next=NULL;
		return ptr;
	}
}

void append_userID (userID *new,userID *ptr)
{
	ptr->next=new->next;
	new->next=ptr;

	new=ptr;
	end=ptr;
}

void print_userIDs (userID *head)
{
	userID *userID;
	userID=head->next;

	 while (userID!=end) {
		printf(",%s,%d\n",userID->uname,userID->ukey);
		userID=userID->next;
	}
}

void delete_userIDs (userID *first)
{
	struct userID *current,*next;
	current=first;

	for (next=current->next;next!=end;next=current->next) {
		current=next;
		free (current);
	}
}
#include <malloc.h>
#include <pthread.h>


/* Process queued jobs until the queue is empty. */
void* thread_function (void* arg)
{
	// open the log file

	while (1) {
		struct job* next_job;

		/* Lock the mutex on the job queue. */
		pthread_mutex_lock (&job_queue_mutex);

		/* Now it’s safe to check if the queue is empty. */
		if (job_queue == NULL)
			next_job = NULL;
		else {
			/* Get the next available job. */
			next_job = job_queue;

			/* Remove this job from the list. */
			job_queue = job_queue->next;
		}

		/* Unlock the mutex on the job queue because 
		we’re done with the queue for now. */
		pthread_mutex_unlock (&job_queue_mutex);

		/* Was the queue empty? If so, end the thread. */
		if (next_job == NULL) {
			// set pthread_cond to wait for signal on queue

			break;
		}
		/* Carry out the work. */
		process_job (next_job);
			/* timestamp
			 	write to file
				printf(); */

		/* Clean up. */
		free (next_job);
	}
	return NULL;
}

void enqueue_job (struct job* new_job)
{
	pthread_mutex_lock (&job_queue_mutex);
	new_job->next = job_queue;
	job_queue = new_job;
	pthread_mutex_unlock (&job_queue_mutex);
}



void initialize_flag ()
{
	// Initialize the mutex and condition variable. 
	pthread_mutex_init (&thread_flag_mutex, NULL);
	pthread_cond_init (&thread_flag_cv, NULL);

	/* Initialize the flag value. */
	thread_flag = 0;
}

/* Calls do_work repeatedly while the thread flag is set; blocks if
the flag is clear. */
void* thread_function (void* thread_arg)
{
	/* Loop infinitely. */
	while (1) {
		/* Lock the mutex before accessing the flag value. */
		pthread_mutex_lock (&thread_flag_mutex);
		while (!thread_flag)
			/* The flag is clear. Wait for a signal on the condition
			variable, indicating that the flag value has changed. When the
			signal arrives and this thread unblocks, loop and check the
			flag again. */
			pthread_cond_wait (&thread_flag_cv, &thread_flag_mutex);

		/* When we’ve gotten here, we know the flag must be set. Unlock
		the mutex. */
		pthread_mutex_unlock (&thread_flag_mutex);
		/* Do some work. */
		do_work ();
	}
	return NULL;
}
/* Sets the value of the thread flag to FLAG_VALUE. */
void set_thread_flag (int flag_value)
{
	// create log(job) for queue

	// wake up logging thread to process queue

	/* Lock the mutex before accessing the flag value. */
	pthread_mutex_lock (&thread_flag_mutex);

	/* Set the flag value, and then signal in case thread_function is
	blocked, waiting for the flag to become set. However,
	thread_function can’t actually check the flag until the mutex is
	unlocked. */
	thread_flag = flag_value;
	pthread_cond_signal (&thread_flag_cv);

	/* Unlock the mutex. */
	pthread_mutex_unlock (&thread_flag_mutex);
}


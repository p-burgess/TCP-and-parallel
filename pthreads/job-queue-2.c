#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>

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

typedef struct job {
  /* Link field for linked list. */
  struct job* next;

  /* Other feilds describing work to be done. */
  char *msg;
  int id;
} job;

typedef struct arg_t {
  int fd;//FILE *fd;
} arg_t;

void initialize_job_queue ();
void* logger (void* arg_t);
void* producer (void* arg_t);
void* enqueue_job (char *message, int id);

/* A linked list of pending jobs. */
/* The queue is intially empty */
struct job *job_head;
struct job *job_end;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex;

/* A semaphore counting the number of jobs in the queue. */
sem_t job_queue_count;

int main ()
{
  //	job *fifo;
  pthread_t log, pro;
  const char *filename="./log.text";
  struct arg_t *arg;
  int log_write;
  
  if ((arg = (arg_t *) malloc (sizeof (arg_t))) == NULL) {
      fprintf (stderr,"Unable to malloc arg");
      exit(0);
  }
  
  if ((arg->fd = (FILE *) malloc (sizeof (FILE))) == NULL) {
      fprintf (stderr,"Unable to malloc arg->fd");
      exit(0);
  }

  log_write = open (filename,O_WRONLY,O_APPEND|O_CREAT);
  
  if (log_write == -1);//NULL);
  {
    fprintf (stderr,"Unable to open log file for appending");
    exit (0);
  }
  
  arg->fd = log_write;  

  initialize_job_queue ();
  
  pthread_create (&pro, NULL, producer, NULL);
  pthread_create (&log, NULL, logger, arg);
  pthread_join (pro, NULL);
  pthread_join (log, NULL);

  return 0;
}

void initialize_job_queue ()
{
  job_head = (struct job *)NULL;
  job_end = (struct job *)NULL;

  pthread_mutex_init(&job_queue_mutex, NULL);

  /* Initialize the senaphore which counts jobs in
  the queue. Its intitial value should be zero. */
  sem_init (&job_queue_count, 0, 0);
}


/* Add a new job to the front of the job queue. */
void *enqueue_job (char *message, int id/* Pass job-specific data here... */)
{
  struct job *ptr;
  
  ptr = (struct job *) calloc (1,sizeof (struct job));
  ptr->msg = (char *) calloc (1, sizeof (message));
  
  if (ptr == NULL)                 /* error allocating node?      */
    return (struct node *) NULL;   /* then return NULL, else      */
  else {                           /* allocated node successfully */	
    strcpy (ptr->msg, message);    /* fill in name details        */
    ptr->id = id;                 /* copy id details             */
  }

  /* Lock the mutex on the job queue before accessing it. */
  pthread_mutex_lock (&job_queue_mutex);

  /* inserts a new node                                          */
  if (job_head == NULL) {              /* if an empty list,          */
    job_head = ptr;                    /* set 'head' to it           */
    job_end = ptr;
    job_head->next = NULL;             /* set end of list to NULL    */
//    return;                        /* and finish                 */
  }

  ptr->next = job_head;             /* link next field to original list   */
  job_head = ptr;                   /* head adjusted to new node          */
 
  /* Post to the semaphore to indicate that another job
  is available. If threads are blocked, waiting on the 
  semaphore, one will become unblocked so it can process
  the job. */
  sem_post (&job_queue_count);

  /* Unlock the job queue mutex. */
  pthread_mutex_unlock (&job_queue_mutex);
  
  return NULL;
}
  
/* Process queued jobs until the queue is empty. */
void* logger (void* arg)
{
  struct job* next_job;
  struct tm * Time;	
  time_t currentTime;
  arg_t *args;
  /*FILE **/int log_write;
  char buffer[256];
  int j;
  
  args = arg;
//  log_write = args->fd;

  
    // create timestamp
  currentTime=time (NULL);
  Time = localtime ( &currentTime );
  
  j = sprintf (buffer,":%d:",Time->tm_year+1900);
  j += sprintf (buffer+j,"%d:",Time->tm_mon+1);
  j += sprintf (buffer+j,"%d:",Time->tm_mday+1);
  j += sprintf (buffer+j,"%d:",Time->tm_hour);
  j += sprintf (buffer+j,"%d:",Time->tm_min);
  j += sprintf (buffer+j,"%d:",Time->tm_sec);
  j += sprintf (buffer+j,"logger started\n");
  // write log entry
  write (log_write,buffer,j);
  
  while (1) {
  // create timestamp
  currentTime=time (NULL);
  Time = localtime ( &currentTime );

  /* Wait on the job queue semaphore. If its value
  is positive, indicating that the queue is not empty,
  decrement the count by 1. If the queue is empty,
  block until a new job is enqueued. */
  sem_wait (&job_queue_count);

  /* Lock the mutex on the job queue. */
  pthread_mutex_lock (&job_queue_mutex);

  /* Because of the semaphore, we know the queue is not
  empty. Get the next available job. */
  next_job = job_end;
  if (next_job == NULL)
    printf("end of list NULL");
  
  
//    struct node *temp;
//  temp = ptr;    /* node to be deleted */
//  temp = head;   /* start of the list, will cycle to node before temp    */
				     /* we deleting first node  */
  job_end = job_end->next;                 /* moves head to next node     */
  
//  if( end == temp ) 	             /* is it end, only one node?   */
//    end = end->next;                 /* adjust end as well          */
//  return temp;
      //free( temp );                   /* free space occupied by node */


  /* Unlock the mutex on the job queue because we're
  done with the queue for now. */
  pthread_mutex_unlock (&job_queue_mutex);

  /* Carry out the work. */
  //		process_job (next_job);
  /* timestamp log messages */
  j = sprintf (buffer,":%d:",Time->tm_year+1900);
  j += sprintf (buffer+j,"%d:",Time->tm_mon+1);
  j += sprintf (buffer+j,"%d:",Time->tm_mday+1);
  j += sprintf (buffer+j,"%d:",Time->tm_hour);
  j += sprintf (buffer+j,"%d:",Time->tm_min);
  j += sprintf (buffer+j,"%d:",Time->tm_sec);
  j += sprintf (buffer+j,"from %d:\n",next_job->id);  
  j += sprintf (buffer+j,"%s:",next_job->msg);
  
  // write log entry
  write (log_write,buffer,j);
  
  // Clean up
  free (next_job);
  
  j = 0;

  }
  
  return NULL;
}

void* producer (void* arg_t)
{
  char *msg;
  int i;
   
  msg = (char *) malloc (256 * sizeof (char));

  strcpy("message", msg);
  
  for (i=0;i<5;i++){
    enqueue_job (msg,getpid());
  }
  
  sleep(5);

  return NULL;
}

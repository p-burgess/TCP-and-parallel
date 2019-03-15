#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NODE_MAX 6
#define NODE_LIM 7
#define DEST 6

#define DEST_NODE 3

typedef struct Node  {
   struct Node *next;
   double resistance;
   int a;
   int b;
} Node;
   
typedef  struct List {
   Node *head;
   Node *tail;
} List;

typedef  struct Step {
   struct Step *next;
   int node;
} Step;

typedef  struct Path {
   Step *head;
   Step *tail;
} Path;

void queue_init(List *myroot) 
{
	myroot->head = NULL;
	myroot->tail = NULL;
}

void queue_put(List *myroot, Node *mynode) 
{
	mynode->next = NULL;
	if (myroot->tail != NULL)
		myroot->tail->next = mynode;
	myroot->tail = mynode;
	if (myroot->head == NULL)
		myroot->head = mynode;
}

Node *queue_get(List *myroot)
{
	//get from root
	Node *mynode;
	mynode = myroot->head;
	if (myroot->head != NULL)
		myroot->head = myroot->head->next;
	return mynode;
}

void path_init(Path *myroot) 
{
	myroot->head = NULL;
	myroot->tail = NULL;
}

Step *path_get(Path *myroot)
{
	//get from root
	Step *mystep;
	mystep = myroot->head;
	if (myroot->head != NULL)
		myroot->head = myroot->head->next;
	return mystep;
}


int path_find(Path *myroot, Step *mynode)
{
	int ret=0;
	Step *mystep;
	mystep = myroot->head;
	while (mystep != NULL) {
		if (mystep->node == mynode->node) ret = -1;
		mystep = mystep->next;
	}
	return ret;
}

void path_put(Path *myroot, Step *mynode) 
{
	mynode->next = NULL;
	if (myroot->tail != NULL)
		myroot->tail->next = mynode;
	myroot->tail = mynode;
	if (myroot->head == NULL)
		myroot->head = mynode;
}

Path* path_clone(Path *myroot)
{
	Path *newroot;
	Step *mystep, *newstep;
//	if (myroot->head == NULL)
//		return NULL;

	newroot = (Path *) malloc (sizeof(Path));
 	path_init(newroot);


	printf("Path head :%d\n",myroot->head->node);
//	printf("cloned path head :%d\n",newroot->head->node);
	mystep = myroot->head;


	do {
		newstep = (Step *) malloc (sizeof(Step));
//		newstep->step = (int *) malloc (sizeof(int));



//		newstep->step = memcpy(mystep,newstep,sizeof(Step));

//		memcpy (newstep->step,mystep->step,sizeof(int));

//		printf("cloned entry :%d\n",mystep->step);

		path_put(newroot, newstep);
		mystep = mystep->next;
	} 	while (mystep->next != NULL);
	return newroot;
}

//this could be done in a for loop using; List *myroot
void adj_put(unsigned int adj[], Node *mynode)
 { 
	unsigned int rowa = 0;	//	
	unsigned int rowb = 0;

	/* Move the mask bit to the correct column and row */
	rowa=(1<<(mynode->a));
	rowb=(1<<(mynode->b));

//	printf("mask bit, nodea :%u\n",rowa);
//	printf("mask bit, nodeb :%u\n",rowb);

	/* Place adjusted bits into the adjacency matrix */
	adj[mynode->b] += rowa;
	
	/* If the node is connected to itself, only place it in once */
	if (mynode->a != mynode->b)
		adj[mynode->a] += rowb;
}



/*
void adj_get(int i)
{
	return A[i];
}
*/



execl("./path_branch","1","4","7","98 37 10 52 40 91 33","4");

// number of steps
// steps list :
// number of adjacency sums
// adjacency sum list :
// this step
	



void *path_branch (Path *myroot, unsigned int adj[], int mybranch)
{
	int n;
	int pid;
	int ret;
	int adj_sum;
	Step *S;
	Path *Clone;

	adj_sum = adj[mybranch];

	S = (Step *) malloc (sizeof(Step));
	S->node = (mybranch+1);

	// check if the node number is already on the path
	// check the list for this entry (n)

	ret = path_find(myroot,S);
	//printf("return from path_find :%d\n",ret);

	if (ret==0) {	// the node is not on the list

		//	fork processes with Clone paths


		// traverse the bit places of the integer from the adjacency matrix
		// corresponding to this node A[mybranch], at each occurance, create
		//  a new path list from the old list, add this node to the path, as
		//	a step, fork a process with this new
		//	node as the next step

		for (n=0;n<=NODE_MAX;n++) {
			if ((adj_sum&(1<<n)) >= 1) { //entry in adjacency matrix

				pid=fork();

				printf("pid :%d\n",pid);

				if (pid==0) {

					// create new path for next iteration
					Clone = path_clone (myroot);		// clone the path
					printf("returned head from clone :%d\n",Clone->head->node);

					// safe to put new step on this path
					path_put(Clone,S);					// put node on the path

					printf("node : %d connected to node :%d\n",mybranch,(n+1));
					path_branch(Clone,adj,n);
					exit (0);
				} else {
					// parent process
//					sleep (3);
//					exit (0);
				}
			}
		}
	} else {

		// print return message
		printf("circular path\n");
		while (myroot->head != NULL) { /*local P[]*/
			S = path_get(myroot);
			printf(":%d ",S->node);
			free (S);
		}
		printf("\n");

	}/* else if (ret==1) {	// node is a destination
	} else if (ret==2) { 
	}*/
	//return;
}


void print_row(unsigned int A) {
	int a;
	int b;
	int x;
	for (x=NODE_MAX;x>=0;x--) {
		a=(1<<x);
		if ((b=(A&a))>=1)
			printf("1  ");
		else
			printf("0  ");
	}
	printf("\t%u\n",A);
}

FILE *fr;            /* declare the file pointer */

int main()

{
	double R;
	int node1;
	int node2;
	int entries = 0;
	int n,x;
	unsigned int a;
	Node *N;
	List *L;
	Step *S;
	Path *P, *Clone;
	int pid;
	int So;
	int Si;

	/* Initialize Adjacency matrix */
	unsigned int A[NODE_LIM];
	for (n=0;n<=NODE_MAX;n++) {
		A[n]=0;
	}
	
	//long elapsed_seconds;
	char line[80];

	/* Check that the file exists */

	/* "rt" means open the file for reading text */
	fr = fopen ("./nodedata.txt", "rt");
	
	L = (List *) malloc (sizeof(List));
	queue_init(L);


	// Initialize Path array. 
	//		Each entry in the array will represent a node,
	//		from which a path may begin.
/*	for (n=0;n<=NODE_MAX;n++) {
		// place initial step 
	}*/

	
	// the first line of the file will be 
	// the source and sink node.
	// format : int int
	// fgets(line, 80, fr);
	// sscanf (line, "%d %d", &So, &Si);
	/* get a line, up to 80 chars from fr.  done if NULL */
	while(fgets(line, 80, fr) != NULL) {

		/* Copy data from file into local memory */
		sscanf (line,"%lf %d %d", &R, &node1, &node2);

		N = (Node *) malloc (sizeof(Node));

		// check if the node is a source or sink
		/*if (node1==node2) { // node has source/sink attached 
			 add source or sink to source/sink list 
			}
			*/
			
		/* Prepare struct for insertion into linked list */
		N->a = node1-1;
		N->b = node2-1;
		N->resistance = R;

//		printf("read entry :%d\t%d\t%lf\n",node1,node2,R);
		
		/* Append resistor to the linked list  */
		queue_put(L, N);
		
		/* put the node entry into the adjacency matrix */
		adj_put(A, N);
		
		// check the addition of the rows

		entries++;
   }
    
	fclose(fr);  /* close the file prior to exiting the routine */
	
	// Print Adjancecy matrix
	printf("adjacency matrix and row sums: \n");
	for (n=0;n<=NODE_MAX; n++) {
//		a = *A[n];
//		printf("sum :%d\n",A[n]);
		print_row (A[n]);
	}
	
	// SPF trial
	// for each node:
	n = 4;


	// construct command line arguement
	char* arg;
	pid = fork();
	if (pid == 0) {	// child
		execl("./path_branch","1","4","7","98 37 10 52 40 91 33","4");
			// arg0 - length of path (number of steps)
			// arg1 - list of nodes on path
			//	arg2 - number of integers in adjacency matrix
			// arg3 - space seperated adjacency sums
			// arg4 - next step
	}

/*
	P = (Path *) malloc (sizeof(Path));
 	path_init(P);

	path_branch (P,A,n);
*/
	//return;
}
/*
	S = (Step *) malloc (sizeof(Step));
	S->node = n;

// need the former path and the adjancency sum to branch the path
// branch at node corresponding to sum, i.e. sum (node 1) = adj[node1]
// node1 - n



//	path_put(P,S);

	//		Within each process, search the adjacency
	//		table for a	connected node,

	// 		fork a child process 
	//		
//	path_branch(P,A,1);


	sleep (3);
	// Print the Node list
	//printf("node list: \n");
			
    exit (0);
} /*of main*/

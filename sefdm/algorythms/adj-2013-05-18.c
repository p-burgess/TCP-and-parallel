#include <stdio.h>
#include <stdlib.h>

#define NODE_LIM 7

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

//this could be done in a for loop using; List *myroot
void adj_put(unsigned int adj[NODE_LIM], Node *mynode)
 { 
	unsigned int row = 1;
	unsigned int col = 1;

	/* Move the mask bit to the correct column and row */
	row=(row<<(mynode->a)-1);
	col=(col<<(mynode->b)-1);

	if (!(row & adj[(mynode->b) -1]) || !(col & adj[(mynode->a) -1])) { 
		// there exists an entry in the adja0cency matrix corresponding 
		// to a this resistor. This is a duplicate.

		/* Place adjusted bits into the adjacency matrix */
		adj[(mynode->b -1)] += row;
		adj[(mynode->a -1)] += col;
	}
}		

void print_row(unsigned int A) {
	int a;
	for (int x=NODE_LIM;x>=0;x--) {
		a=(1<<x);
		if ((A&a)>=1)
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
	int n;
	Node *N;
	List *L;
	int pid;
	int So;
	int Si;

	/* Initialize Adjacency matrix */
	unsigned int A[NODE_LIM];
	for (n=0;n<=NODE_LIM;n++) {
		A[n]=0;
	}
	
	//long elapsed_seconds;
	char line[80];

	/* "rt" means open the file for reading text */
	fr = fopen ("./nodedata.txt", "rt");

	L = (List *) malloc (sizeof(List));
	queue_init(L);
	
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
		N->a = node1;
		N->b = node2;
		N->resistance = R;
		N->next = NULL;
//		printf("read entry :%d\t%d\t%lf\n",node1,node2,R);
		
		/* Append resistor to the linked list  */
		queue_put(L, N);
		
		/* put the node entry into the adjacency matrix */
		adj_put(A, N);

		entries++;
		
		for (i=0;i<=NODE_LIM;i++) {
		// if ((A[0]&&i) > 0) { //entry in adjacency matrix
			pid=fork();
			if (pid==0) {
				printf("this is child :%d", (1<<i));
				// child prrocess
				exit (0);
			}
		}
				
		
		
		
		
		
		
		
		
		
    }
    
	fclose(fr);  /* close the file prior to exiting the routine */
	
	// Print Adjancecy matrix
	printf("adjacency matrix and row sums: \n");
	for (n=0;n<NODE_LIM; n++) {
		print_row ( A[n]);
	}
	
	// Print the Node list
	//printf("node list: \n");
			
    exit (0);
} /*of main*/
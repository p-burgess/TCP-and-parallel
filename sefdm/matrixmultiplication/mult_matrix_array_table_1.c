//http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml
// this program create a random matrix
// and a rwnfom vector, then subtract
//  the vector from a specific row in the
//  matrix, replacing the row with the
//  result.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define Eb 2
#define NUM_ROWS 4
#define NUM_COLS 10

#define MODULATION_LEVEL 8
#define ALPHABET_SIZE 4

#define PI 3.14159265

typedef struct node {
	struct node *next_col;
	double imag;
	double real;
	unsigned int row;
	unsigned int col;
} node;

void node_init (node *mynode, double in_real, double in_imag,unsigned int in_row,unsigned int in_column)
{
	mynode->real=in_real;
	mynode->imag=in_imag;
	mynode->row=in_row;
	mynode->col=in_column;
}

void print_node (node *mynode)
{
	printf("%d , %d : ",mynode->row,mynode->col);
	printf("%.10e :\t%.10e\t\n",mynode->real,mynode->imag);	
	return;
}

void node_copy (node *newnode,node *mynode)
{
	node_init (newnode,mynode->real,mynode->imag,mynode->row,mynode->col);
	return;
}

/*
typedef struct list {
 char *tag;
 unsigned int length;
 node **entry;
} list;

void list_init (list *mylist,char *name,unsigned int in_length)
{
	mylist->tag = (char *) malloc (16*sizeof(char));
	strncpy (mylist->tag,name,16);
	mylist->length=in_length;
	mylist->entry=(node **) malloc (in_length*(sizeof(node)));
}
*/

/*
void list_append (list *mylist,node *mynode)
{
	mynode->next=NULL;
	if (mylist->tail!=NULL)
		mylist->tail->next=mynode;
	mylist->tail=mynode;
	if (mylist->head==NULL)
		mylist->head=mynode;
}
*/

/*
void list_insert (list *mylist,node *mynode,unsigned int row,unsigned int col)
{
	mynode->next=NULL;
	if (mylist->tail!=NULL)
		mylist->tail->next=mynode;
	mylist->tail=mynode;
	if (mylist->head==NULL)
		mylist->head=mynode;
}
*/

/*
void print_list (list *mylist)
{
	//unsigned int i;
	node *mynode;
	
	printf("\n\n");
	printf("%s\tlength : %d\n",mylist->tag,mylist->length);
	printf("%s : \nrow col   real\t\timag\n",mylist->tag);
	mynode=mylist->head;
		while (mynode!=NULL) {
		printf("%d , %d :\t%.10e\t%.10e\t\n",mynode->row,mynode->col,mynode->real,mynode->imag);
		mynode=mynode->next; 
	}
	
	return;
}
*/

/*
void create_random_list (list *mylist,unsigned int length)
{
 unsigned int i;
 double real,imag;
 node *mynode;
 
 for (i=0;i<length;i++) {
	mynode = (node *) malloc (sizeof(node));
	real = (double) rand()/(double)RAND_MAX-1.0;
	imag = (double) rand()/(double)RAND_MAX-1.0;
	node_init (mynode,real,imag,0,i);
	list_append (mylist,mynode);
  }

 //for (i=0;i<size;i++) x_0[i] = 1.0;
// for (i=0;i<size;i++) x_0[i] = (unsigned int) rand() % MODULATION_LEVEL;
 return;
}
*/

/*
typedef struct array {
	char *tag;
	unsigned int length;
	node **entry;
} array;

void array_init (array *myarray,unsigned int length,char *name)
{
	myarray->length=length;
	myarray->entry = (node **) malloc (length*sizeof(node));
	myarray->tag = (char *) malloc (16*sizeof(char));
	strncpy (myarray->tag,name,16);
	return;
}

void print_array (array *myarray)
{
	unsigned int i;
	node *mynode;
	
	printf("\nrow col   real\t\t\timag\n");
	for (i=0;i<myarray->length;i++) {
		mynode = myarray->entry[i];
		print_node (mynode);
	}
	return;
}

void array_insert (array *myarray,node *mynode)
{
	myarray->entry[mynode->col]=mynode;
	return;
}

void create_random_array (array *myarray)
{
 unsigned int i;
 double real,imag;
 node *mynode;
 
 for (i=0;i<myarray->length;i++) {
	mynode = (node *) malloc (sizeof(node));
	real = (double) rand()/(double)RAND_MAX-1.0;
	imag = (double) rand()/(double)RAND_MAX-1.0;
	node_init (mynode,real,imag,0,i);
	array_insert (myarray,mynode);
  }

 //for (i=0;i<size;i++) x_0[i] = 1.0;
// for (i=0;i<size;i++) x_0[i] = (unsigned int) rand() % MODULATION_LEVEL;
 return;
}
*/

typedef struct table 
{
	char *tag;
    unsigned int rows;
    unsigned int columns;
    //double *sum;
	//double norm;
	// char[16] name;
    node **col;
} table;

void table_init (table *mytable,int in_rows,int in_columns,char *name)
{
	mytable->tag = (char *) malloc (16*sizeof(char));
	strncpy (mytable->tag,name,16);
	int i;
	// mytable->name = tag;
	// snprint,  memcopy
	//mytable->norm=0.0;
	mytable->rows=in_rows;
	mytable->columns=in_columns;
	//mytable->sum = (double *) malloc (in_columns*sizeof(double));
	mytable->col = (node **) malloc (in_columns*sizeof(node *));
	//mytable->tail = (node **) malloc (in_columns*sizeof(node));
	for (i=0;i<in_columns;i++) {
		mytable->col[i]= (node *) malloc (in_rows*sizeof(node));
		//mytable->tail[i]=NULL;	
	}
}

/*
#include<stdio.h>
#include<stdlib.h>
  
int main()
{
    int r=3, c=4;
    int **arr;
    int count = 0,i,j;
  
    arr  = (int **)malloc(sizeof(int *) * r);
    arr[0] = (int *)malloc(sizeof(int) * c * r);
 
    for(i = 0; i < r; i++)
        arr[i] = (*arr + c * i);
  
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            arr[i][j] = ++count;  // OR *(*(arr+i)+j) = ++count
  
    for (i = 0; i <  r; i++)
        for (j = 0; j < c; j++)
            printf("%d ", arr[i][j]);
  
    return 0;
}

*/
void table_insert (table *mytable, node *mynode) 
{
	mytable->col[mynode->col][mynode-row]mynode->next=NULL;
	if (mytable->tail[mynode->col]!=NULL)
		mytable->tail[mynode->col]->next=mynode;
	mytable->tail[mynode->col]=mynode;
	if (mytable->head[mynode->col]==NULL)
		mytable->head[mynode->col]=mynode;
}

void print_table (table *mytable)
{
	unsigned int i;
	node *mynode;
	
	printf("\n\n");
	printf("%s\trows : %d\tcolumns : %d\n",mytable->tag,mytable->rows,mytable->columns);
	for (i=0;i<mytable->columns;i++) {
		printf("%s[x,%d] : \nx , col	 real\t\t\timag\n",mytable->tag,i);
		mynode=mytable->head[i];
		while (mynode!=NULL) {
			print_node (mynode);
			mynode=mynode->next; 
		}
	}
	return;
}

void table_mult (table *C,table *A,table *B)
{
	//C = A x B : A E R^nxm, B E R^mxn, C E R^mxn,  c ij = 0
	unsigned int j,k;
	node *anode,*bnode,*cnode;
	for (k=0;k<A->columns;k++) {
		for (j=0;j<B->columns;j++) {
			anode = A->head[k];
			bnode = B->head[j];
			cnode = C->head[j]; 
			while (bnode != NULL && cnode != NULL) {
				cnode->real += anode->real*bnode->real - anode->imag*bnode->imag;
				cnode->imag += anode->real*bnode->imag + anode->imag*bnode->real;
				bnode = bnode->next;
				cnode = cnode->next;
				anode = anode->next;
			}
		}
	}
	return;
}

void create_random_matrix (table *mytable)
{
	unsigned int i,k;
	double real,imag;
	node *mynode;
	
	for (i=0;i<mytable->columns;i++) {
			for (k=0;k<mytable->rows;k++) {
				mynode = (node *) malloc (sizeof(node));
				real = 2.0*(double) rand()/(double)RAND_MAX-1.0;
				imag = 2.0*(double) rand()/(double)RAND_MAX-1.0;
				node_init (mynode,real,imag,k,i);
				table_append (mytable,mynode);
			}
	}
}

void create_pivot_matrix (table *mytable, unsigned int x)
{
	unsigned int i;
	double real;
	double imag;
	node *pivot;
	node **pivot_row;
	
	pivot_row = (node**) malloc (mytable->rows*(sizeof(node*));
	for (i=0;i<mytable->rows;i++)
	{
		while (pivot->row[i
	
	// get pivot row
	
	// get the piovt
	pivot=mytable->head[x];
	while(pivot->row != x) pivot=pivot->next;
	
	
}
/*
void mmult_row_x_by_vector (table *mytable,unsigned int row,array *myarray)
{
	unsigned int i;
	double real,imag;
	//node *mynode;
	for (i=0;i<length;i++) {
		real = row[i]->real*node->real - row[i]->imag*node->imag;
		imag = row[i]->real*node->imag + row[i]->imag*node->real;
		row[i]->real = real;
		row[i]->imag = imag;
		//areal = Ar[i]->real*conj->real - Ar[i]->imag*conj->imag;
		//aimag = Ar[i]->real*conj->imag + Ar[i]->imag*conj->real;
		//Ar[i]->real = areal;
		//Ar[i]->imag = aimag;
		
	}
}
*/
int main()
{
	srand (time(NULL));
	//int i,j;
	//double sig;

	array *VECTOR=(array *) malloc (sizeof(array));
	array_init(VECTOR,NUM_COLS,"VECTOR");
	create_random_array (VECTOR);
	
	table *MATRIX = (table *) malloc (sizeof(table));
	table_init (MATRIX,NUM_ROWS,NUM_COLS,"MATRIX");
	create_random_matrix (MATRIX);
	
	
	create_pivot_matrix (table *mytable,unsigned int pivot);
	
	

	printf("\n\n");
	printf("NUM_COLUMNS :\t\t%d \n",NUM_COLS);
	printf("NUM_ROWS :\t\t%d \n",NUM_ROWS);
	//printf("MODULATION_LEVEL :\t%d \n",MODULATION_LEVEL);

	
	print_table (MATRIX);
	print_array (VECTOR);
	
	/*
	print_table (IP_SIG);
	print_table (CARRIERS);
	print_table (CORRELATION);
	print_table (MODI);
	print_table (AWGN);
	print_table (NOISY);
	print_table (INVERSE);
	*/
	
	return 0;
}
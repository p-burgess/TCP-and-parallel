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
	struct node *next;
	unsigned int row;
	unsigned int col;
	double imag;
	double real;
} node;

void node_init (node *mynode, double in_real, double in_imag, int in_row, int in_col)
{
	mynode->row=in_row;
	mynode->col=in_col;
	mynode->real=in_real;
	mynode->imag=in_imag;
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

void node_normalize (node *mynode,node *a,node *b,double divisor)
// mynode = a x b
{
	double real,imag;
	real = (a->real*b->real - a->imag*b->imag)/divisor;
	imag = (a->real*b->imag + a->imag*b->real)/divisor;
	node_init (mynode,real,imag,a->row,a->col);
}

typedef struct list {
 char *tag;
 unsigned int length;
 node *head;
 node *tail;
} list;

void list_init (list *mylist,char *name)
{
	mylist->tag = (char *) malloc (16*sizeof(char));
	strncpy (mylist->tag,name,16);
	mylist->length=0;
	mylist->head=NULL;
	mylist->tail=NULL;
}

void list_append (list *mylist,node *mynode)
{
	mynode->next=NULL;
	if (mylist->tail!=NULL)
		mylist->tail->next=mynode;
	mylist->tail=mynode;
	if (mylist->head==NULL)
		mylist->head=mynode;
}

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

// an array is a pointer to pointers to nodes (array[i]->entry),
// with some associated meta-data (size,name; array->length)
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

void array_insert (array *myarray,node *mynode, unsigned int position)
{
	myarray->entry[position]=mynode;
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
	array_insert (myarray,mynode,i);
  }

 //for (i=0;i<size;i++) x_0[i] = 1.0;
// for (i=0;i<size;i++) x_0[i] = (unsigned int) rand() % MODULATION_LEVEL;
 return;
}

typedef struct table 
{
	char *tag;
    unsigned int rows;
    unsigned int columns;
    double *sum;
	double norm;
	// char[16] name;
    node **head;
	node **tail;
} table;

void table_init (table *mytable,int in_rows,int in_columns,char *name)
{
	mytable->tag = (char *) malloc (16*sizeof(char));
	strncpy (mytable->tag,name,16);
	int i;
	// mytable->name = tag;
	// snprint,  memcopy
	mytable->norm=0.0;
	mytable->rows=in_rows;
	mytable->columns=in_columns;
	mytable->sum = (double *) malloc (in_columns*sizeof(double));
	mytable->head = (node **) malloc (in_columns*sizeof(node*));
	mytable->tail = (node **) malloc (in_columns*sizeof(node*));
	for (i=0;i<in_columns;i++) {
		mytable->head[i]=NULL;
		mytable->tail[i]=NULL;	
	}
}

void table_append (table *mytable, node *mynode) 
{
	mynode->next=NULL;
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

void create_random_matrix (table *mytable)// need to input size
{
	unsigned int i,k;
	double real,imag;
	node *mynode;
	
	for (i=0;i<mytable->columns;i++) {
			for (k=0;k<mytable->rows;k++) {
				mynode = (node *) malloc (sizeof(node));
				real = 1.0*(double) rand()/(double)RAND_MAX-1.0;
				imag = 1.0*(double) rand()/(double)RAND_MAX-1.0;
				node_init (mynode,real,imag,k,i);
				table_append (mytable,mynode);
			}
	}
}


void matrix_reduce_row (table *mytable, unsigned int x)
// expects an augmented matrix and row number of diagonal
{
	unsigned int i;
	double real;
	double imag;
	double divisor;
	node *pivot,*conjugate;
	node **pivot_row,**pivot_column,**normal_row;
	node *cursor,*target;
	
	// allocate memory to hold normalized vector
	normal_row = (node **) malloc (2*mytable->columns*sizeof(node*));
	for (i=0;i<(2*mytable->columns);i++) {
		normal_row[i] = (node*) malloc (sizeof(node));
	}
	pivot_column = (node**) malloc (mytable->rows*sizeof(node*));
	for (i=0;i<(mytable->rows);i++) {
		pivot_column[i] = (node*) malloc (sizeof(node));
	}
	
	// find pivot, calculate divisor
	cursor = mytable->head[x];
	while (cursor != NULL) {
		if (cursor->row = x) break;
		cursor = cursor->next;
	}
	divisor = abs((cursor->real*cursor->real) - (cursor->imag*cursor->imag));
	real = cursor->real/divisor;
	imag = (-1)*cursor->imag/divisor;
	
	// allocate memory to hold conjugate of diagonal element
	conjugate = (node*) malloc (sizeof(node));
	node_init (conjugate,real,imag,x,x);
		
	// normalize the pivot row; multiply each element by the 
	// conjugate, divide by divisor, put result into normal_row 
	for (i=0;i<(2*mytable->columns);i++) {
		// find row element
		cursor = mytable->head[i];
		while (cursor!=NULL) {
			if (cursor->row = x) break;
			cursor=cursor->next; 
		}
		node_normalize (normal_row[i],cursor,conjugate,divisor);
	}
	
	// populate pivot column
	cursor = mytable->head[x];
	i=0;
	while (cursor != NULL) {
		node_init (pivot_column[i++],cursor->real,cursor->imag,cursor->row,x);
		cursor = cursor->next;
	}
	// lets see where were at
	
	
	/*
	pivot = pivot_row[x];
	pivot_column = mytable->head[x];
	
	for (i=0;i<mytable->rows;i++)
	{
		cursor = mytable->head[i];
		target = outtable->head[i];
		while (cursor != NULL) {
			if (cursor->row != x) {
				real = pivot_row[1]->real*cursor->real+pivot_row[i]->imag*cursor->imag;
				imag = pivot_row[1]->real*cursor->imag-pivot_row[i]->imag*cursor->real;
				memcpy (cursor->real,&real,sizeof(double));
				memcpy (cursor->imag,&imag,sizeof(double));
			} else {
				;
			}
			cursor = cursor->next;
			
		}
	}
	// get pivot row
	
	// get the piovt
	pivot=mytable->head[x];
	while (pivot->row != x) pivot=pivot->next;	
	*/
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
	printf("/nhere/n");
	
	srand (time(NULL));
	//int i,j;
	//double sig;

	
	array *VECTOR=(array *) malloc (sizeof(array));
	array_init(VECTOR,NUM_COLS,"VECTOR");
	create_random_array (VECTOR);
	
	table *MATRIX = (table *) malloc (sizeof(table));
	table_init (MATRIX,NUM_ROWS,NUM_COLS,"MATRIX");
	create_random_matrix (MATRIX);
	
	table *AUGMENT = (table *) malloc (sizeof(table));
	table_init (AUGMENT,NUM_ROWS,(2*NUM_COLS),"AUGMENT");
	//augment_matrix (MATRIX,AUGMENT);
	//table *SUBTRAHEND = (table *) malloc (2*sizeof(table));
	//table_init (SUBTRAHEND,NUM_ROWS,NUM_COLS*2,"SUBTRAHEND");
		
	//matrix_reduce_row (AUGMENT,0);
	
	printf("\n\n");
	printf("NUM_COLUMNS :\t\t%d \n",NUM_COLS);
	printf("NUM_ROWS :\t\t%d \n",NUM_ROWS);
	//printf("MODULATION_LEVEL :\t%d \n",MODULATION_LEVEL);

	
	print_table (MATRIX);
	print_table (AUGMENT);
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
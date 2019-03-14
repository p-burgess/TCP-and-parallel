//http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define Eb 2
#define NUM_CARRIERS 5
#define MODULATION_LEVEL 8
#define ALPHABET_SIZE 4
#define NUM_BLOCKS 10
#define ALPHA 1
#define OVERSAMPLING 2
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
	printf("\n%d , %d : ",mynode->row,mynode->col);
	printf("%.10e :\t%.10e\t\n",mynode->real,mynode->imag);	
	return;
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
	mytable->head = (node **) malloc (in_columns*sizeof(node));
	mytable->tail = (node **) malloc (in_columns*sizeof(node));
	for (i=0;i<in_columns;i++) {
		mytable->head[i]=NULL;
		mytable->tail[i]=NULL;	
	}
}

void node_copy (node *newnode,node *mynode)
{
	node_init (newnode,mynode->real,mynode->imag,mynode->row,mynode->col);
	return;
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
		printf("%s[x,%d] : \n	: real\t\t: imag\n",mytable->tag,i);
		mynode=mytable->head[i];
			while (mynode!=NULL) {
			printf("%d , %d :\t%.10e :\t%.10e\t\n",mynode->row,mynode->col,mynode->real,mynode->imag);
			mynode=mynode->next; 
		}
	}
	return;
}

void print_array (node **myarray,int size)
{
	unsigned int i;
	node *mynode;
	
	printf("\nrow col   real\t\t\timag\n");
	for (i=0;i<size;i++) {
		mynode = myarray[i];
		printf("%d , %d :\t%.10e\t%.10e\t\n",mynode->row,mynode->col,mynode->real,mynode->imag);
	}
	return;
}


void table_sub (table *C,table *A,table *B)
{
	//C = A - B : A,B,C Ɛ R^n*m, C empty
	unsigned int i;
	node *anode,*bnode,*cnode;
	//table *C;
	
	
	//C = (table *) malloc (sizeof (table));
	//table_init (C,A->rows,A->columns);
	// find the matrix with the least number of columns 
	// to use as the end value in the for statement.
	for (i=0;i<C->columns;i++) {
		anode = A->head [i];
		//print_node (anode);
		bnode = B->head [i];
		//print_node (bnode);
		cnode = C->head [i];
		while (anode != NULL && bnode != NULL) {
			// check anode->row == bnode->row !: increment node
			cnode->real = anode->real - bnode->real;
			cnode->imag = anode->imag - bnode->imag;
			cnode->row = anode->row;
			cnode->col = anode->col;
			anode = anode->next;
			bnode = bnode->next;
			cnode = cnode->next;
		}
	}
	return;
}


void table_mult (table *C,table *A,table *B)
{
	//C = A x B : A E R^nxm, B E R^mxn, C E R^mxn,  c ij = 0
	unsigned int i,j,k;
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

unsigned int *random_X (int size)
{
 unsigned int i;
 unsigned  int *x_0 = (int *) malloc (size*sizeof(unsigned int));
 //for (i=0;i<size;i++) x_0[i] = 1.0;
 for (i=0;i<size;i++) x_0[i] = (unsigned int) rand() % MODULATION_LEVEL;
 return x_0;
}

void create_IP_BLOCKS (table *myip_sig,unsigned int *X,double alpha,double pi)
{
	double real;
	double imag;
	double a;
	unsigned int i,j,k=0;
	unsigned int b;

	node *mynode;
	for (i=0;i<NUM_BLOCKS;i++) {
		for (j=0;j<NUM_CARRIERS;j++) {	
			mynode = (node *) malloc (sizeof(node));
			/*
				8-QAM, Gray Mapping
				
			*/
			a = 1;
			b = X[k++];
			if ((b & (1<<0)) != 0) {real = sin(0.205*pi)*(-1);}//2*pi*ALPHA*(i%n))
			else if ((b & (1<<0)) == 0) {real = sin(0.205*pi);}//2*pi*ALPHA*(i%n));
			if ((b & (1<<1)) != 0) {a = 0.333333;}
			if ((b & (1<<2)) != 0) {imag = cos(0.205*pi)*a;}//2*pi*ALPHA*(i%n))
			else if ((b & (1<<2)) == 0) {imag = cos(0.205*pi)*(-1)*a;}
			node_init (mynode, real, imag,j,i);
			table_append (myip_sig, mynode);
		}
	}
}

void create_CARRIERS (table *mycarrier)
{
	unsigned int i,j;
	double real,imag;

	node *mynode;

	for (i=0;i<mycarrier->columns;i++) {
		for (j=0;j<mycarrier->rows;j++) {
			mynode = (node *) malloc (sizeof(node));
			real = sin(2*PI*ALPHA*(i-1)*j/mycarrier->rows)/sqrt(mycarrier->rows);
			imag = cos(2*PI*ALPHA*(i-1)*j/mycarrier->rows)/sqrt(mycarrier->rows);
			
			node_init (mynode,real,imag,j,i);
			table_append (mycarrier,mynode);
		}
	}
}

void TRANSPOSE (table *mytable,table *newtable)
{
	unsigned int i;
	node *mynode,*newnode;
	
	for (i=0;i<mytable->columns;i++) {
		mynode = mytable->head[i];
		while (mynode != NULL) {
			newnode = (node *) malloc (sizeof(node));
			node_init (newnode,mynode->real,mynode->imag,mynode->col,mynode->row);
			table_append (newtable,newnode);
			mynode = mynode->next;
		}
	}
}

void correlate (table *mycarrier,table *newtable)
{
	// 	B = A' * A;
	// Calculate the product of the Hermitian transpose
	// of the matrix(A) and itself, place result(B) in
	// newtable
	unsigned int i,j;
	double real_sum,imag_sum;
	unsigned int b;
	node *xnode,*ynode,*mynode;

	for (j=0;j<mycarrier->columns;j++) {
		for (i=0;i<mycarrier->columns;i++) {
			real_sum = 0;
			imag_sum = 0;
			/*if (i==j){
				real_sum = 1.0;
				imag_sum = 1.0;
			}else {*/
			xnode=mycarrier->head[j];
			ynode=mycarrier->head[i];
			while (xnode!=NULL && ynode!=NULL) {
				real_sum += (xnode->real*ynode->real + xnode->imag*ynode->imag);
				imag_sum += (xnode->real*ynode->imag - ynode->real*xnode->imag);
				xnode=xnode->next;
				ynode=ynode->next;
			}
			/*}*/
			mynode = (node *) malloc (sizeof(node));
			node_init (mynode,real_sum,imag_sum,i,j);
			table_append (newtable, mynode);
		}
	}
	return;
}

void modulate (table *mytable,table *mycarriers,table *myip_sig)
{
	unsigned int i,j,k;
	double real,imag;
	
	node *mynode,**Anode,*xnode;
	
	for (k=0;k<myip_sig->columns;k++){
	
		Anode = (node **) malloc (mycarriers->columns*sizeof(node));

		for (i=0;i<mycarriers->columns;i++) {
			Anode[i] = mycarriers->head[i];
		}

		//check length of vector == number of rows
		
		// the ->norm is used to store the Frobenius norm of the complex space,
		// this value is used in noise calculations.
		
		for (i=0;i<mycarriers->rows;i++) {
			real=0.0;
			imag=0.0;
			j=0;
			xnode = myip_sig->head[k];
			while (xnode!=NULL) {
				real += Anode[j]->real*xnode->real+Anode[j]->imag*xnode->imag;
				imag += Anode[j]->real*xnode->imag+Anode[j]->imag*xnode->real;
				Anode[j]=Anode[j++]->next;
				xnode=xnode->next;
			}
			mytable->norm += (real*real)+(imag*imag);
			mynode = (node *) malloc (sizeof(node));
			node_init (mynode,real,imag,i,k);
			table_append (mytable,mynode);
		}
	}
	real = sqrt(mytable->norm);
	mytable->norm=real;
}

double sigma (table *mymodi) 
{
	unsigned int i;
	double mysigma = 0.0;
	node *mynode;
	
	for (i=0;i<mymodi->columns;i++) { // global !
		mynode=mymodi->head[i];
		while (mynode!=NULL) {
			mysigma += OVERSAMPLING* ((mynode->real*mynode->real) + (mynode->imag*mynode->imag));
			mynode=mynode->next;
		}
	}
	return sqrt((mysigma/(pow(10,Eb/10)*log(MODULATION_LEVEL)*NUM_BLOCKS*NUM_CARRIERS))/2);
}

void create_noise (table *mytable,double EbNo,double sigma,int oversampling)
{
	unsigned int i,j,k,l=0;
	double real,imag;
	node *mynode;
	
	for (i=0;i<mytable->columns;i++) {
			for (k=0;k<mytable->rows;k++) {
				mynode = (node *) malloc (sizeof(node));
				real = 2.0*sigma*(double) rand()/(double)RAND_MAX-1.0;
				imag = 2.0*sigma*(double) rand()/(double)RAND_MAX-1.0;
				node_init (mynode,real,imag,k,i);
				table_append (mytable,mynode);
			}
	}
}


void addition (table *C,table *A,table *B)
{
	// C = A + B
	unsigned int i,j;
	double real,imag;
	node *a,*b,*c;
	// check that rows==rows and cols==cols
	//printf ("\nB_rows : %d\tB_cols : %d",B->rows,B->columns);
	//	printf ("\nmod : %d\t",mod);
	
	for (i=0;i<OVERSAMPLING;i++) {
		for (j=0;j<B->columns;j++) {
			a = A->head[j+(i*OVERSAMPLING)];
			b = B->head[j];
			 while ( b != NULL && a != NULL ) {
			 	real = a->real + b->real;
			 	imag = a->imag + b->imag;
			 	//This may not work in distributed system. the table should be allocated before being sent.
			 	c = (node *) malloc (sizeof(node));
			 	node_init (c, real,imag,a->row,i+(j*OVERSAMPLING));
			 //	printf ("\na_col : %d\ta_row : %d\n",c->col,c->row);
			 	table_append (C,c);
			 	a=a->next;
			 	b=b->next;
			 }
		}
	}
	//printf ("\nrow : %d\tcol : %d\treal :\f%.4lf\timag  :\t%.5lf", c->row,c->col,c->real,c->imag);
	return;
}

void identity_init (table *mytable)
{
	node *mynode;
	unsigned int i,j;
	double real,imag;
	for (j=0;j<mytable->columns;j++) {
		for (i=0;i<mytable->rows;i++) {
			mynode = (node *) malloc (sizeof(node));
			real = 0.0;
			imag = 0.0;
			if (i == j) real = 1.0;
			node_init (mynode,real,imag,i,j); 
			table_append (mytable,mynode);
		}
	}
}

void zero_init (table *mytable)
{
	node *mynode;
	unsigned int i,j;
	double real,imag;
	for (j=0;j<mytable->columns;j++) {
		for (i=0;i<mytable->rows;i++) {
			mynode = (node *) malloc (sizeof(node));
			node_init (mynode,0.0,0.0,i,j); 
			table_append (mytable,mynode);
		}
	}
}

double mag (node *mynode)
{
	double a,b,result;
	a = mynode->real;
	b = mynode->imag;
	
	result = sqrt (a*a + b*b);
	//printf ("\n %.20e ^2 + %.20e ^2 = %.20e ^2",a,b,result);
	return result;
}

node *divide (node *numnode,node *dennode)
{
	node *mynode = (node *) malloc (sizeof(node));
	double den,real,imag;
	
	den = dennode->real*dennode->real-dennode->imag*dennode->imag;
	real = numnode->real*dennode->real - numnode->imag*dennode->imag;
	imag = numnode->imag*dennode->real - numnode->real*dennode->imag;
	
	node_init (mynode,real,imag,numnode->row,numnode->col);
	return mynode;	
}

void copy_row_from_table (node **array,table *mytable,int row)
{
	unsigned int i;
	node *mynode;//,*newnode;

	for (i=0;i<mytable->columns;i++) {
		//printf ("\n%d",i);
		
		mynode = mytable->head[i];
		while (mynode != NULL) {
			if (mynode->row == row) {
				node_copy (array[i],mynode);
				break;
			}
			mynode = mynode->next;	
		}
	}
	
	//printf ("\n");
	return;
}

void copy_row_to_table (table *mytable,node **row,unsigned int x)
{
	unsigned int i;
	node *mynode;//,*newnode;
	//printf ("\n%d\n",x);
	for (i=0;i<mytable->columns;i++) {
		mynode = mytable->head[i];
		while (mynode != NULL) {
			if (mynode->row == x) {
				//print_node (mynode);

				mynode->real = row[i]->real;
				mynode->imag = row[i]->imag;
			
				//print_node (row[i]);
				//print_node (mynode);
				break;
			}
			mynode = mynode->next;	
		}
	}
	return;
}

node **get_column_from_table (table *mytable,int column)
{
	node *mynode,**cnodes;
	cnodes = (node **) malloc ((mytable->rows)*sizeof(node *));

	mynode = mytable->head[column];
		while (mynode != NULL) {
			cnodes[mynode->row] = mynode;
			mynode = mynode->next;	
		}
	return cnodes;
}

void print_list (list *mylist)
{
	unsigned int i;
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

int switch_b (node *anode,node *bnode) 
{
	if (anode->row == bnode->row) return 0;
	if (anode->row > bnode->row) return 1;
	if (anode->row < bnode->row) return -1;
}


void swap (table *mytable,int r,int j)
{
	node *mynode,*prevnode,*jnode,*jprevnode,*rnode,*rprevnode,*tempnode;
	node *anode; //make a linked list of denominators for next stage
	
	unsigned int i;	
	
	// 2: interchange rows r and j, store row a(i,r) for next step
	for (i=0;i<mytable->columns;i++) { //all columns swap
		prevnode=mytable->head[i];
		mynode=mytable->head[i];
		while (mynode != NULL) {
			if (mynode->row == j) {
				jnode=mynode;
				jprevnode=prevnode;
			}
			if (mynode->row == r) {
				rnode=mynode;
				rprevnode=prevnode;
			}
			prevnode=mynode; // since j > 1
			mynode=mynode->next;
		}
		if (jnode == mytable->tail[j] || rnode == mytable->tail[r]) {
			mytable->tail[j]=rnode;
			mytable->tail[r]=jnode;
		}

		// conditions:
		// swap
		jnode->row=r;
		rnode->row=j;		
		jprevnode->next=rnode;
		rprevnode->next=jnode;
		tempnode=jnode->next;
		jnode->next=rnode->next;
		rnode->next=tempnode;
	}
	//print_table (mytable);
	// interchange columns r and j.
	tempnode=mytable->head[r];
	mytable->head[r]=mytable->head[j];
	mytable->head[j]=tempnode;
	
	tempnode=mytable->tail[r];
	mytable->tail[r]=mytable->tail[j];
	mytable->tail[j]=tempnode;
	
	mynode=mytable->head[j];
	tempnode=mytable->head[r];
	while (mynode != NULL && tempnode != NULL) {
		mynode->col=j;
		tempnode->col=r;
		mynode=mynode->next;
		tempnode=tempnode->next;
	}
	//print_table (mytable);
	return;
}
/*		// 2: do the same for identity matrix; split into functions
	}
}	
		
		// 3: compute multiplier n(i,r) for i:r+1,N-1 where n=a(i,r-1)/a(r,r-1)
		// multiplier is a linked list of complex values from r+1 to N-1
		anode=ar->head;
		
		//int blocks = 2*mytable->rows;

		
		//compute_multiplier_0 (n,mytable,r,N);
		
		list *multiplier = (list *) malloc (sizeof(list));
		list_init (multiplier);
		compute_multiplier (multiplier,mytable,r);
		
		
		// 4: Subtraact n(i,r) * row r from row i // need row r sent to following process
		inplace_transform (mytable,multiplier,r);
		free (multiplier);
	}		//**here**
}*/
	
void invert (table *mytable,table *augment)
{
	//print_table (mytable);
	//print_table (augment);
	// problem :  the time it takes to invert the matrix ; parallelism
	
	// Gaussian elimination with pivoting Numerical Recipes p.594
	unsigned int i,r,j=0;
	double S,mag,real,imag,augreal,augimag,rreal,rimag,areal,aimag,pivotreal,pivotimag,rrreal,rrimag,rareal,raimag;
	S=0.0;
	// r is the iteration number
	//	pivot (table,row);
	//		find largest value in column r, swap the 
	//		row in which this occurs with row r, to 
	//		minimize rounding errors.
	
	node *pivotnode,*mynode,*anode,*conj,*xnode,**Rr,**Cr,**Ar,*snode,*asnode;
	
	table *subtr,*asubtr,*A,*Aprime,*Aug,*Augprime;
	
	subtr = (table *) malloc (sizeof(table));
	table_init (subtr,mytable->rows,mytable->columns,"Subtrahend");
	asubtr = (table *) malloc (sizeof(table));
	table_init (asubtr,mytable->rows,mytable->columns,"AugSubtrahend");
	
	A = (table *) malloc (sizeof (table));
	table_init (A,mytable->rows,mytable->columns,"A");
	Aprime = (table *) malloc (sizeof (table));
	table_init (Aprime,mytable->rows,mytable->columns,"Aprime");
	
	Aug = (table *) malloc (sizeof (table));
	table_init (Aug,augment->rows,augment->columns,"Aug");
	Augprime = (table *) malloc (sizeof (table));
	table_init (Augprime,augment->rows,augment->columns,"Augprime");
	
	//copy source matrix to A
	for (i=0;i<mytable->columns;i++) {
		mynode = mytable->head[i];
		while (mynode != NULL) {
			anode = (node *) malloc (sizeof(node));
			node_init (anode,mynode->real,mynode->imag,mynode->row,mynode->col);
			table_append (A,anode);
			mynode=mynode->next;
		}
	}
	
	//copy target matrix to Aug
	for (i=0;i<augment->columns;i++) {
		mynode = augment->head[i];
		while (mynode != NULL) {
			anode = (node *) malloc (sizeof(node));
			node_init (anode,mynode->real,mynode->imag,mynode->row,mynode->col);
			table_append (Aug,anode);
			mynode=mynode->next;
		}
	}
	
	//create dummy table for subtrahend
	for (i=0;i<subtr->rows;i++) {
		for (j=0;j<subtr->columns;j++) {
			snode = (node *) malloc (sizeof(node));
			node_init (snode,0.0,0.0,i,j);
			table_append (subtr,snode);
		}
	}
	//print_table (subtr);
	
	//create dummy table for augmented subtrahend matrix 
	for (i=0;i<asubtr->rows;i++) {
		for (j=0;j<asubtr->columns;j++) {
			snode = (node *) malloc (sizeof(node));
			node_init (snode,0.0,0.0,i,j);
			table_append (asubtr,snode);
		}
	}
	//print_table (asubtr);
	
	
	// create array to hold the pivot row
	Rr = (node **) malloc (mytable->columns*sizeof(node *));
	for (i=0;i<mytable->columns;i++) {
		Rr[i] = (node *) malloc (sizeof(node));
	}
	//print_array (Rr, mytable->columns);
	
	// ..and the pivot row of the augment
	Ar = (node **) malloc (mytable->columns*sizeof(node *));
	for (i=0;i<mytable->columns;i++) {
		Ar[i] = (node *) malloc (sizeof(node));
	}
	//print_array (Ar, mytable->columns);
	
	// conjugate of the pivot 
	conj = (node *) malloc (sizeof(node));
	
	// create a table to hold the evaluated matrix.
	//	(combine process with subtrahend)
	for (i=0;i<Aprime->rows;i++) {
		for (j=0;j<Aprime->columns;j++) {
			mynode = (node *) malloc (sizeof(node));
			node_init (mynode,0.0,0.0,i,j);
			table_append (Aprime,mynode);
		}
	}

	// ..and its augment.
	for (i=0;i<Augprime->rows;i++) {
		for (j=0;j<Augprime->columns;j++) {
			mynode = (node *) malloc (sizeof(node));
			/*if (i == j) {node_init (mynode,1.0,0.0,i,j);}
			else {*/node_init (mynode,0.0,0.0,i,j);//}
			table_append (Augprime,mynode);
		}
	}
	
	for (r=0;r<mytable->columns;r++) {
		
		// Gaussian Elimination
		//	normalize target entry (1st entry = 1 + i0)
		//	divide row by target entry.

		//printf ("\n Matrix iteration : %d",r);

		// 		copy pivot row from matrix and augment
		copy_row_from_table (Rr,A,r);
		copy_row_from_table (Ar,Aug,r);
		print_array (Rr,mytable->columns);
		print_array (Ar,augment->columns);
		
		pivotnode = Rr[r];
		//print_node (pivotnode);
		pivotreal = Rr[r]->real;
		pivotimag = Rr[r]->imag;
		printf ("\n pivotreal:\t%.10e",pivotreal);
		printf ("pivotimag:\t%.10e \n",pivotimag);
		imag = 0.0;
		
		//		if the pivot has an imaginary component, 
		//		multiply that value by -1, to obtain a 
		//		conjugate.
		if (pivotimag != 0) {
			imag = pivotimag*(-1.0);
			//printf ("\n\tpivot node has iaginary component\n")
		//}
			node_init (conj,pivotreal,imag,pivotnode->row,pivotnode->col);
			//print_node (conj);
		
		//		condition row r of matrix and augment by
		// 		multiplying it by the conjugate of the pivot, 
		//		the pivot element will have only a real part 
		//		after this operation
		//if (Rr[r]->imag != 0) {
		// 		multiply row and augment row by conjugate
			for (i=0;i<mytable->columns;i++) {
				real = Rr[i]->real*conj->real - Rr[i]->imag*conj->imag;
				imag = Rr[i]->real*conj->imag + Rr[i]->imag*conj->real;
				Rr[i]->real = real;
				Rr[i]->imag = imag;
				areal = Ar[i]->real*conj->real - Ar[i]->imag*conj->imag;
				aimag = Ar[i]->real*conj->imag + Ar[i]->imag*conj->real;
				Ar[i]->real = areal;
				Ar[i]->imag = aimag;
			}
		}
	
		// Since we have multiplied the pivot row by the conjugate of 
		// the pivot, the pivot will only have a real part. The pivot
		// row will be divided by this part, leaving 1 in the pivot position.
		//pivotnode = Rr[r];
		//printf ("\n\tpivotreal: \t%.10e\n",pivotnode->real);
	
		
		// 		divide row and augmented row by pivotnode->real,
		//		to complete normalization.
		//if (pivotreal != 1.0) {
			for (i=r;i<mytable->columns;i++) {
				// mytable and augment table have the 
				// same number of columns
				//aimag = 0.0;
				rreal = Rr[i]->real;
				rimag = Rr[i]->imag;
				areal = Ar[i]->real;
				aimag = Ar[i]->imag;
				
				printf ("rreal: %.10e\t\trimag: %.10e\n",rreal/pivotreal,rimag/pivotreal);
				rrreal = rreal/pivotreal;
				rrimag = rimag/pivotreal;
				rareal = areal/pivotreal;
				raimag = aimag/pivotreal;
				printf ("rrreal: %.10e\t\trrimag: %.10e\n",rrreal,rrimag);
				
				
				// not setting the pointed to 
				// variable to the new value
				// ... try using function
				Rr[i]->real = rrreal;
				Rr[i]->imag = rrimag;
				Ar[i]->imag = rareal;
				Ar[i]->imag = raimag;
				
				
				
				printf ("Rr[i]->real: %.10e\t\tRr[i]->imag: %.10e\n",Rr[i]->real,Rr[i]->imag);
				// ...divide augennted row by real part of pivot...
				//if (Rr[i]->imag != 0.0) { }
			}
		//}
		//print_array (Rr,mytable->columns);
		//print_array (Ar,augment->columns);
		
		// The normalized pivot row can now replace the original
		// row of the matrix and augment.
		print_array (Rr,mytable->columns);
		print_array (Ar,augment->columns);
		
		copy_row_to_table (A,Rr,r);
		copy_row_to_table (Aug,Ar,r);
		
		//print_table (A);
		//print_table (subtr);
		//print_table (mytable);
		//print_table (augment);
		// The pivot column is copied from updated table.
		// Each indexed element in this column, except the pivot element,
		// will be used as a multiplier for the pivot row. The products
		// are then assembled into a matrix(Subtrahend), with row indices
		// matching the index of the pivot column multiplier. At the end 
		// of this calculation all elements in the pivot column, except
		// the pivot element, are 0. The pivot row is not calculated,
		// since the pivot already has the desired value of 1. 
		Cr = get_column_from_table (A,r);
		//print_table (mytable);
		//print_array (Cr,mytable->rows);
		
		//printf ("\n column\t %d of Matrix",r);
		//print_array (Cr,mytable->rows);
		//printf ("\nhere\n");
		
		// zero subtrahend matrix
		for (i=0;i < r;i++) {
			snode = subtr->head[i];
			while (snode != NULL) {
				snode->real = 0.0;
				snode->imag = 0.0;
				snode=snode->next;
			}
		}
		// zero subtrahend augment
		for (i=0;i < r;i++) {
			snode = asubtr->head[i];
			while (snode != NULL) {
				snode->real = 0.0;
				snode->imag = 0.0;
				snode=snode->next;
			}
		}
		
		//Compute Subtrahend:
		for (i=r;i < A->columns;i++) {
			// start at column r, column by column

			// each subtrahend node evaluates to a 
			// new value with Cr and Rr
			mynode = A->head[i];
			snode = subtr->head[i];
		
			//	multiply the normalized element of pivot row, corresponding
			//  to this column's index, by element in the pivot column, 
			//  whose row index corresponds to the row index of this column.
			while (mynode != NULL && snode != NULL) {
				if (snode->row != r) {
					//this value is computed mynode - (Rr(r,i)*Cr(r,r))
					snode->real = Cr[mynode->row]->real*Rr[mynode->col]->real-Cr[mynode->row]->imag*Rr[mynode->col]->imag;
					snode->imag = Cr[mynode->row]->real*Rr[mynode->col]->imag+Cr[mynode->row]->imag*Rr[mynode->col]->real;
					//mynode->real = real;
					//mynode->imag = imag;
				}  else  {
					snode->real = 0.0;
					snode->imag =0.0;
				}
				//print_node (snode);
				mynode = mynode->next;
				snode = snode->next;
			}
			//  place the product with row index equal to the row index in 
			//  the pivot column and this column's index.
		}
		
		//printf ("\nhere\n");
		
		
		//Compute Subtrahend for augmented table
		for (i=0;i <= r;i++) {
			// start at column r, column by column

			mynode = Aug->head[i];
			asnode = asubtr->head[i];
			
			//print_node (mynode);
			//print_node (asnode);
			
			//	multiply the normalized element of augmented pivot row, corresponding
			//  to this augmented column's index, by element in the pivot column, 
			//  whose row index corresponds to the row index of this augmented column.
			while (mynode != NULL && asnode != NULL) {
				if (asnode->row != r) {
					//this value is computed mynode - (Rr(r,i)*Cr(r,r))
					asnode->real = Cr[mynode->row]->real*Ar[mynode->col]->real+Cr[mynode->row]->imag*Ar[mynode->col]->imag;
					asnode->imag = Cr[mynode->row]->real*Ar[mynode->col]->imag+Cr[mynode->row]->imag*Ar[mynode->col]->real;
				}  else  {
					asnode->real = 0.0;
					asnode->imag =0.0;
				}
				//print_node (asnode);
				mynode = mynode->next;
				asnode = asnode->next;
				//  place the product with row index equal to the row index of
				//  the pivot column and this augmented column's index.
			}
		}
		//print_table (Aug);
		//print_table (asubtr);

		
		// subtract subtrahend matrix, augmented subtrahend from
		// augmented matrix.
		table_sub (Aprime,A,subtr);
		table_sub (Augprime,Aug,asubtr);
		//print_table (Aug);
		//print_table (Aprime);
		//print_table (Augprime);
		A = Aprime;
		Aug = Augprime;
		//print_table (Aug);
		//free (conj);
	}
	// At step r = 1, 2, 3 .. N-1
	// 1: find the row with the largest element in column r-1, below the diagonal, (r')
	//	if zero, go to 4,
	// 2: swap rows  r' and r, swap columns r' and r
	// 3: subtract a - a n*a(r)
	//strncpy (augment->tag,"INVERSE",16);
	//print_table (Aug);
	// copy result(Aug) to augment
	for (i=0;i<augment->columns;i++) {
		mynode = augment->head[i];
		anode = Aug->head[i];
		while (mynode !=NULL && anode != NULL) {
			mynode->real = anode->real;
			mynode->imag = anode->imag;
			mynode = mynode->next;
			anode = anode->next;
		}
	}
	return;
}

int main()
{
	srand (time(NULL));
	int i,j;
	double sig;


	unsigned int *X = random_X (NUM_BLOCKS*NUM_CARRIERS);

	table *IP_SIG = (table *) malloc (sizeof(table));
	table_init (IP_SIG,NUM_CARRIERS,NUM_BLOCKS,"IP_SIG");
	
    create_IP_BLOCKS (IP_SIG,X,ALPHA,PI);



	table *CARRIERS = (table *) malloc (sizeof(table));
	table_init (CARRIERS,NUM_BLOCKS,NUM_CARRIERS,"CARRIERS");

	create_CARRIERS (CARRIERS);
	//print_table (CARRIERS);
	


	//table *CARRIERST = (table *) malloc (sizeof(table));
	//table_init (CARRIERST,NUM_CARRIERS,NUM_BLOCKS,"CARRIERST");

	//TRANSPOSE (CARRIERS,CARRIERST);
	//print_table (CARRIERST);
	
	//printf("\nrows : %d\tcolumns : %d",CARRIERST->rows,CARRIERST->columns);
	
	
	table *CORRELATION = (table *) malloc (sizeof(table));
	table_init (CORRELATION,CARRIERS->columns,CARRIERS->columns,"CORRELATION");
	correlate (CARRIERS,CORRELATION);
	//print_table (CORRELATION);
	

	table *MODI = (table *) malloc (sizeof(table));
	table_init (MODI,NUM_BLOCKS,NUM_BLOCKS,"MODI");
	modulate (MODI,CARRIERS,IP_SIG);
	//print_table (MODI);
	
	sig = sigma (MODI); 

	table *AWGN = (table *) malloc (sizeof(table));
	table_init (AWGN,NUM_BLOCKS*OVERSAMPLING,NUM_BLOCKS,"AWGN");
	create_noise (AWGN,Eb,sig,OVERSAMPLING);
	//print_table (AWGN);
	
	table *NOISY = (table *) malloc (sizeof(table));
	table_init (NOISY,NUM_BLOCKS,NUM_BLOCKS*OVERSAMPLING,"NOISY");
	addition (NOISY,AWGN,MODI);
	//print_table (NOISY);

	table *INVERSE = (table *) malloc (sizeof(table));
	table_init (INVERSE,CORRELATION->rows,CORRELATION->columns,"INVERSE");
	identity_init (INVERSE);

	
	invert (CORRELATION,INVERSE);
	//print_table (INVERSE);	
	
	table *ANSWER = (table *) malloc (sizeof(table));
	table_init (ANSWER,INVERSE->rows,INVERSE->columns,"ANSWER");
	zero_init (ANSWER);
	
	table_mult (ANSWER,CORRELATION,INVERSE);
	//print_table (ANSWER);	
	// check inversion of matrix
	


// SYM_RX=CARRIERS'*SEFDM_SYMBOL_NOISY1;
// SYM_RX_EVEN = correlation\SYM_RX; % MATLAB function : inverse of correlation matrix; then multiply with SYM_RX
//      SYM_RX_EVEN_REAL=SYM_RX_EVEN_REAL-(SYM_RX_EVEN_REAL==0);
      //SYM_RX_EVEN_IMAG=SYM_RX_EVEN_IMAG-(SYM_RX_EVEN_IMAG==0);
      
//      ZF =SYM_RX_EVEN_REAL+1i*SYM_RX_EVEN_IMAG;

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
     

//       numZF=symerr(IP_BLOCK,ZF,'row-wise'); % MATLAB function : compare IP_BLOCK and ZF, line by line
//       ber_ZF_candidate(:,EbNo) = ber_ZF_candidate(:,EbNo) +  numZF/No_blocks  ;
             
//  end
  
  
	
	/*for (i=0;i<NUM_BLOCKS;i++) {
		o = column_from_table (IP_SIG,i);
		Ax (CARRIERS,SYMBOL,IP_BLOCK);
		list_to_table (MODI,SYMBOL,i);	
	}*/
	

	//inverse (CORRELATION,INVERSE);
		
	
	// send result to table :end of loop calculate sigma
	
	
	/*list *IP_BLOCK = (list *) malloc (sizeof(list));
	list_init (IP_BLOCK);*/
		
	
	//	printf("\t%d\t%d\t:\t%.5lf\t%.5lf\t\n",mynode->col,mynode->row,mynode->real,mynode->imag);

	//table *SYMBOL_MODI = create_SYMBOL_MODI (CARRIERS,IP_BLOCK);

	//list *SEFDM_SYMBOL = (list *) malloc (sizeof(list));
	//A_x (CARRIERS,SEFDM_SYMBOL,IP_BLOCK);

	//double SEFDM_SQUARED = complex


	
	printf("\n\n");
	printf("NUM_CARRIERS :\t\t%d \n",NUM_CARRIERS);
	printf("MODULATION_LEVEL :\t%d \n",MODULATION_LEVEL);
	printf("NUM_BLOCKS :\t\t%d \n",NUM_BLOCKS);
	printf("ALPHA :\t\t\t%d \n",ALPHA);
	printf("OVERSAMPLING :\t\t%d \n",OVERSAMPLING);
	printf("Eb :\t\t\t%d \n",Eb);
	
	//print_table (CARRIERS);
	//print_table (CORRELATION);
	
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
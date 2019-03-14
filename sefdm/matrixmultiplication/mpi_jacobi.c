
/*

http://users.abo.fi/mats/PP2012/examples/MPI/

A simple MPI example program using broadcast and non-blocking receive
with MPI_Waitany, modified to approximate the solution to of a system
of equations, using the Jacobi method, and implemented with linked 
lists. This system describes a network of conductances connected at 
one of two nodes, to a source, and at the other a drain. 

./a

*/
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>
#include "mpi.h"
#include <assert.h>
//#define n 100
/* from: http://mpi.deino.net/mpi_functions/MPI_Reduce.html */

typedef struct node {
 struct node *next;
 double G;
 int row;
 int col;
} node;

void node_init (node *mynode, double in_G, int in_row, int in_col)
{
 mynode->next=NULL;
 mynode->G=in_G;
 mynode->row=in_row;
 mynode->col=in_col;
}

typedef struct list {
 int length;
 double *diag;
 node *head;
 node *tail;
} list;

void x_init (int size, double *x)
{
 int i;
 for (i=0;i<size;i++) x[i]=0.0;
}

void list_init (int size, list *mylist)
{
 mylist->diag = (double *) malloc (size*sizeof(double));
 mylist->length=0;
 mylist->head=NULL;
 mylist->tail=NULL;
}

void list_append (list *mylist, node *mynode)
{
 mynode->next=NULL;
 if (mylist->tail!=NULL)
  mylist->tail->next=mynode;
 mylist->tail=mynode;
 if (mylist->head==NULL)
  mylist->head=mynode;
 mylist->length++;
 if (mynode->row==mynode->col)
  mylist->diag[mynode->row]=mynode->G;
}

node *get_node (list *mylist, int x)
{
 int i=0;
 node *mynode;
 mynode=mylist->head;
 while (mynode!=NULL) {
  if (i++==x) break;
  mynode=mynode->next; }
 return mynode;
}

double *A_jaco_x (int size, list *mylist, double *x)
{
 int i=0;
 node *mynode;
 double *A_x = (double *) malloc (size*sizeof(double));
 x_init (size,A_x);
 do {
  mynode = get_node(mylist,i++);
  if (mynode->row!=mynode->col) {
   A_x[mynode->row] += mynode->G*x[mynode->col];}}
 while (mynode!=mylist->tail);
 return A_x;
}

double *x_sub_y (int size, double *x, double *y)
{
 int i;
 double *x_sub_y = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) x_sub_y[i]=x[i]-y[i];
 return x_sub_y;
}

double *x_add_y (int size, double *x, double *y)
{
 int i;
 double *x_add_y = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) x_add_y[i]=x[i]+y[i];
 return x_add_y;
}

double *x_div_y (int size, double *x, double *y)
{
 int i;
 double *xOy = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) xOy[i]=x[i]/y[i];
 return xOy;
}
double *copy_x (int size,double *x)
{
 int i;
 double *cop_x = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) cop_x[i]=x[i];
 return cop_x;
}

double *x_diff_y (int size, double *x, double *y)
{
 int i;
 double *diffs = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) diffs[i]=fabs(x[i]-y[i]);
 return diffs;
}

double max_x (int size, double *x)
{
 int i;
 double max;
 for (i=0;i<size;i++) if (x[i]>max) max=x[i];
 return max;
}


double *create_B(int n) {
 double *B = (double *) malloc (n*sizeof(double));
 assert(B != NULL);
 int i;
 B[0]=10.0;
 for (i=1;i<(n-1);i++) {
  B[i]=0.0;//B[i]= (double) rand()/RAND_MAX;
 }
 return B;
}

double *create_X(int n) {
 double *X = (double *) malloc (n*sizeof(double));
 assert(X != NULL);
 int i;
 for (i=1;i<(n-1);i++) {
  if (i!=0 && i!=(n-1))
   X[i]=1.0;//X[i]= (double) rand()/RAND_MAX;
 }
 X[0]=10.0;
 X[(n-1)]=0.0;
 return X;
}

list *create_A (int size, int first)
{
 int i=0,j=0;
 list *A = (list *) malloc (sizeof(list));
 list_init(size,A);
 node *mynode;
 for (i=0;i<size;i++) {
  for (j=0;j<size;j++) {
   if (i==(j-1)||i==(j+1)) {
    mynode = (node *) malloc (sizeof(node));
    node_init (mynode,1.0,first+i,first+j);
    list_append (A,mynode);
    A->diag[i]+=1.0; }
   if (i==j) {;/*
    mynode = (node *) malloc (sizeof(node));
    node_init (mynode,2.0,i,i); // if i,j mynode->i,j = size,size - why is that?
    list_append (A,mynode);*/ }}}
 return A;
}

int main(int argc, char* argv[]) 
{
 int n =16,i,j,iter,p, me,size;
 MPI_Init(&argc, &argv);                /* Initialize MPI */
 MPI_Comm_size(MPI_COMM_WORLD, &p);    /* Get nr of processes */
 MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */
 size = n/p;
 double sum;
 double a[size][size];
 double t_diff,m_diff,m_error=0.0;
 for (i=0;i<size;i++) {
  sum=0.0;
  for (j=0;j<size;j++) {
   if (i==(j-1)||i==(j+1)) a[i][j]=1.0;
   else a[i][j]=0.0;
   sum+=a[i][j]; } a[i][i]=sum;}
 double x[size],x_k[size],X_k[n];
 double *X,*B;
 if (me==0) {
  X = create_X (n);m_error=0.0;
  B = create_B (n);
 } else {
  X = (double *) malloc (n*sizeof(double));
  B = (double *) malloc (n*sizeof(double));
 }
 MPI_Bcast(B,size*p,MPI_DOUBLE,0,MPI_COMM_WORLD);
 MPI_Bcast(X,size*p,MPI_DOUBLE,0,MPI_COMM_WORLD);
 MPI_Scatter(X,size,MPI_DOUBLE,x,size,MPI_DOUBLE,0,MPI_COMM_WORLD);
 iter=0.0;
 for (i=0;i<size;i++) { sum=0.0;
  for (j=0;j<size;j++) {
   if (i!=j) sum+=a[i][j]*x[j];}
  x_k[i]=(B[i]-sum)/a[i][i];
  if (i!=0&&me!=0) x_k[i]=10.0;
  if (i!=(size-1)&&me!=(p-1)) x_k[i]=0.0;
  t_diff=fabs(X[((me*size)+i)]-x_k[i]);
  if (t_diff>m_diff) m_diff=t_diff;
  } ++iter;
 for (i=0;i<size;i++) {
  printf("\t%d: %d\t%.3lf",me,i,x_k[i]);}
 MPI_Allgather(x_k,size,MPI_DOUBLE,X_k,size,MPI_DOUBLE,MPI_COMM_WORLD);
 MPI_Reduce(&m_diff,&m_error,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
 if (me==0) {
  printf("\nProcess: %d\terror:%.3lf\n",me,m_error);
 }
 //if (me==2) {
 //for (i=0;i<size;i++) {
  //printf("\t%d:%d: %.3lf",me,i,x_k[i]);}
 //}
 printf("\n");
 MPI_Finalize();
 exit (0);
}

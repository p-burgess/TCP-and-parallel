/*

A program to approximate the solution to of a system of equations,
using the Gauss-Seidel method implemented with linked lists. The 
system describes a network of conductances connected at one of two
nodes, to a source, and at the other a drain. 

./a

--  non-working  --

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define max_its  100

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

double *x_sub_y (int size, double *x, double *y)
{
 int i;
 double *x_sub_y = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) x_sub_y[i]=x[i]-y[i];
 return x_sub_y;
}

double *copy_x (int size,double *x)
{
 int i;
 double *new_x = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) new_x[i]=x[i];
 return new_x;
}

double x_y (int size, double *x, double *y)
{
 int i;
 double sum=0.0;
 for (i=0;i<size;i++) sum+=x[i]*y[i];
 return sum;
}

double *x_div_y (int size, double *x, double *y)
{
 int i;
 double *xOy = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) xOy[i]=x[i]/y[i];
 return xOy;
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

double *A_back_x (int size, list *mylist, double *x, double *b)
{
 int i,j,k=0;
 node *mynode;
 double *A_x = (double *) malloc (size*sizeof(double));
 double *left_x = (double *) malloc (size*sizeof(double));
 double right_x;
 x_init (size,A_x);
 double a[size][size];
 do {
  mynode = get_node(mylist,k++);
  i = mynode->row;
  j = mynode->col;
  a[i][j]=mynode->G;
  if (i < j && i != j) A_x[i] +=mynode->G*x[j];
 } while (mynode!=mylist->tail);
 k=0;
 left_x[0]=(b[0]-A_x[0])/mylist->diag[0];
 for (i=1;i<size;i++) {
  right_x = A_x[i];
  for (j=0;j<i;j++) {
   right_x+=left_x[j]*a[i][j]; }
  left_x[i]+=(b[i]-right_x)/mylist->diag[i]; }
 return left_x;
}

double *x_add_y (int size, double *x, double *y)
{
 int i;
 double *x_add_y = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) x_add_y[i]=x[i]+y[i];
 return x_add_y;
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
 
double *create_x_0 (int size, int range)
{
 int i;
 double *x_0 = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) x_0[i] = 1.0;//range * (double) rand()/RAND_MAX;//1.0;
 return x_0;
}

double *create_b (int size, double S)
{
 int i;
 double *b = (double *) malloc (size*sizeof(double));
 for (i=0;i<size-1;i++) b[i] = 0.0;//range * (double) rand()/RAND_MAX;//1.0;
 b[size-1] = S;
 return b;
}

list *create_A (int size)
{
 int i=0,j=0;
 list *A = (list *) malloc (sizeof(list));
 list_init(size,A);
 node *mynode;
 for (i=0;i<size;i++) {
  for (j=0;j<size;j++) {
   if (i==(j-1)||i==(j+1)) {
    mynode = (node *) malloc (sizeof(node));
    node_init (mynode,1.0,i,j);
    list_append (A,mynode); }
   if (i==j) {
    mynode = (node *) malloc (sizeof(node));
    node_init (mynode,2.0,i,i); // if i,j mynode->i,j = size,size - why is that?
    list_append (A,mynode); }}}
 return A;
}

int main(int argc, char *argv[])
{
 int iter=0;
 int msec;
 clock_t start=clock(),c_diff;
 int n;
 if (argc<2) {
  printf("Usage: command nodes");
  return 1;
 }
 n=atoi(argv[1]);
 srand (time(NULL));
 list *A;
 A = create_A (n-1);
 double *b, *x, *new_x, *diff;
 double error;
 x = create_x_0 (n-1,2.5);
 b = create_b (n-1,10.0);
 new_x = copy_x (n-1,x);
 diff = (double *) malloc ((n-1)*sizeof(double));
 //for (i=0;i<20;i++) {
 do {
  new_x = A_back_x(n-1,A,x,b);
  diff = x_diff_y (n-1,new_x,x);
  error = max_x (n-1,diff);
  x = copy_x (n-1,new_x);
  iter++;
  /*printf("\niteration: %d\terror: %.22lf   ",iter,error);
  fflush(stdout);*/
 } while (error>0.0000001);
 /*printf("\nx[i]:\n");
 for (j=0;j<n-1;j++) printf("%.2lf   ",x[j]);
 fflush(stdout);*/
 c_diff=clock()-start;
 msec=c_diff*1000/CLOCKS_PER_SEC;
 printf("%d\t%d\t%d.%d\n",n,iter,msec/1000,msec%1000);
 fflush(stdout);
 return 0;
}

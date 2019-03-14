#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define n 16 
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

double *A_prod_x (int size, list *mylist, double *x)
{
 int i;
 node *mynode;
 double *A_x = (double *) malloc (size*sizeof(double));
 x_init (size,A_x);
 for (i=0;i<mylist->length;i++) {
  mynode = get_node(mylist,i);
  A_x[mynode->row] += mynode->G*x[mynode->col];}
 return A_x;
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
 
double *create_x_0 (int size, int range)
{
 int i;
 double *x_0 = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) x_0[i] = range * (double) rand()/RAND_MAX;//1.0;
 return x_0;
}

double *create_b (int size, double S)
{
 int i;
 double *b = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) b[i] = 0.0;//range * (double) rand()/RAND_MAX;//1.0;
 b[size-1] = S;
 return b;
}

list *create_A (int size)
{
 int i=0,j=0;
 list *A = (list *) malloc (sizeof(list));
 list_init(n-1,A);
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

int main()
{
 int i,j,k;
 srand (time(NULL));
 list *A;
 A = create_A (n-1);
 /*
 double *b;// = (double *) malloc ((n-1)*sizeof(double));
 b = create_b (n-1,10.0);
 /*for (i=0;i<n-1;i++) {
  if (i==(n-2)) b[i]=10.0;
  else b[i]=0.0; }*/
 double *x;
 x = create_x_0 (n-1,2.5);
 double *Ax,*b_Ax,*new_x,*diff;
 diff = (double *) malloc ((n-1)*sizeof(double));
 double error;
 do {
  //for (i=0;i<500;i++) {
  Ax = A_jaco_x(n-1,A,x);
  /* printf("Ax[i]:\n");
  for (j=0;j<n-1;j++) printf("%.2lf\t",Ax[j]);
  printf("\n");*/
  b_Ax = x_sub_y (n-1, b, Ax);
  /* printf("b-Ax[i]:\n");
  for (j=0;j<n-1;j++) printf("%.2lf\t",b_Ax[j]);
  printf("\n");*/
  new_x = x_div_y (n-1,b_Ax, A->diag);
  /* printf("new_x[i]:\n");
  for (j=0;j<n-1;j++) printf("%.2lf\t",new_x[j]);
  printf("\n");*/
  diff = x_diff_y (n-1,new_x,x);
  error = max_x (n-1,diff);
  //   free(x);
  free(b_Ax);
  free(diff);
  //   x = copy_x (n-1,new_x);
  for (j=0;j<n-1;j++)  x[j]=new_x[j];
  free(new_x);
  printf("iteration: %d\terror: %.22lf   ",k++,error);
  printf("\n");
 //}
 /* printf("\nx[i]:\n");
 for (j=0;j<n-1;j++) printf("%.2lf\t",x[j]);
 printf("\n");*/
 } while (error>0.0001);
 printf("x[i]:\n");
 for (j=0;j<n-1;j++) printf("%.2lf   ",x[j]);
 printf("\n");
/* printf("\nerror: %.2lf\t",error);
 printf("\n");*/
 return 0;
}

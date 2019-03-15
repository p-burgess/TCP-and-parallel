/*

A program to approximate the solution to of a system of equations,
using the conjugate-gradient method, implemented with linked lists.
The system describes a network of conductances connected at one of
two nodes, to a source, and at the other a drain. 

./a []

--  non-working  --
*/

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
 node *head;
 node *tail;
} list;

void list_init (list *mylist)
{
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
}


node *get_node (list *mylist, int x)
{
 int i=0;
 node *mynode;
 mynode=mylist->head;
 while (mynode!=mylist->tail) {
  if (i++==x) break;
  mynode=mynode->next; }
 return mynode;
}

double *A_prod_x (int size, list *mylist, double *x)
{
 int i;
 node *mynode;
 double *A_x = (double *) malloc (size*sizeof(double));
 for (i=0;i<mylist->length;i++) {
  mynode = get_node(mylist,i);
  A_x[mynode->row] += mynode->G*x[mynode->row];}
 return A_x;
}

double *x_prod_A (int size, double *x, list *mylist)
{
 int i;
 node *mynode;
 double *x_A = (double *) malloc (size*sizeof(double));
 for (i=0;i<mylist->length;i++) {
  mynode = get_node(mylist,i);
  x_A[mynode->col] += mynode->G*x[mynode->col];}
 return x_A;
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

double x_y (int size, double *x, double *y)
{
 int i;
 double sum=0.0;
 for (i=0;i<size;i++) sum+=x[i]*y[i];
 return sum;
}

list *c_A (double c, list *mylist)
{
 int i;
 list *cA = (list *) malloc (sizeof(list));
 list_init(cA);
 node *mynode;
 node *Anode;
 for (i=0;i<mylist->length;i++) {
  Anode = get_node(mylist,i);
  mynode = (node *) malloc (sizeof(node));
  mynode->G=c*Anode->G;
  mynode->row=Anode->row;
  mynode->col=Anode->col; }
 return cA;
}

double *c_x (int size, double c, double *x)
{
 int i;
 double *c_x = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) c_x[i]=c*x[i];
 return c_x;
}

double *copy_x (int size,double *x)
{
 int i;
 double *cop_x = (double *) malloc (size*sizeof(double));
 for (i=0;i<size;i++) cop_x[i]=x[i];
 return cop_x;
}

double m_error (int size, double *x, double *y)
{
 int i;
 double t_diff,m_diff;
 for (i=0;i<size;i++) { 
  t_diff=fabs(x[i]-y[i]);
  if (t_diff>m_diff) m_diff=t_diff;}
 return m_diff;
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
 for (i=0;i<size;i++) x_0[i] = 1.0;
 return x_0;
}

list *create_A (int size)
{
 int i=0,j=0;
 list *A = (list *) malloc (sizeof(list));
 list_init(A);
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
 srand (time(NULL));
 int i,j,k,l;
 list *A;
 list *alphaA;
 double *x,*Cx,*Ax,*r,r2,*p,*p_A;
 double alpha,beta,error;
 double *alphap,*new_x,*new_r,*alphaAp;
 double *Bp,*new_p;

double *b = (double *) malloc ((n-1)*sizeof(double));
for (i=0;i<n-1;i++) {
 if (i==(n-2)) b[i]=10.0;
 else b[i]=0.0;
}

 x = create_x_0 (n-1,2.5);//double *) malloc ((n-1)*sizeof(double));
/*printf("\nx[i]:\n");
for (i=0;i<n-1;i++) printf("%.4lf\t",x[i]);
printf("\n");*/

//Cx = c_x (n-1,2.5,x);
/*printf("\nCx[i]:\n");
for (i=0;i<n-1;i++) printf("%.4lf\t",Cx[i]);
printf("\n");*/

 A = create_A (n-1);
/*printf("\nA->length: %d\n",A->length);
node *mynode;
for (i=0;i<A->length;i++) {
 mynode = get_node (A,i);
 printf("item %d: %d\t%d\t%.2lf\n",i,mynode->row,mynode->col,mynode->G);
}*/

 Ax = A_prod_x(n-1,A,x);
/*printf("\nAx[i]:\n");
for (i=0;i<n-1;i++) printf("%.2lf\t",Ax[i]);
printf("\n");*/

 r = x_sub_y (n-1,b,Ax);
/*printf("\nr[i]: b - Ax\n");
for (i=0;i<n-1;i++) printf("%.2lf\t",r[i]);
printf("\n");*/

 p = copy_x (n-1,r);
/*printf("\np[i]:\n");
for (i=0;i<n-1;i++) printf("%.2lf\t",p[i]);
printf("\n");*/

for (i=0;i<10;i++) {
 p_A = x_prod_A (n-1,p,A);
/*printf("\np_A[i]: \n");
for (i=0;i<n-1;i++) printf("%.2lf\t",p_A[i]);
printf("\n");*/

 alpha = x_y (n-1, r, r) / x_y (n-1, p_A, p);
/*printf("\nalpha: %.2lf\t",alpha);
printf("\n");*/

 alphap = c_x (n-1,alpha,p);

 new_x = x_add_y (n-1, x, alphap);
/*printf("\nnew_x[i]: \n");
for (i=0;i<n-1;i++) printf("%.2lf\t",new_x[i]);
printf("\n");*/

 alphaA = c_A (alpha,A);
 alphaAp = A_prod_x (n-1,alphaA,p);

 new_r = x_sub_y (n-1, r, alphaAp);
/*printf("\nnew_r[i]: \n");
for (i=0;i<n-1;i++) printf("%.2lf\t",new_r[i]);
printf("\n");*/

 beta = x_y (n-1, new_r, new_r) / x_y (n-1, r, r);
/*printf("\nbeta: %.2lf\t",beta);
printf("\n");*/

 Bp = c_x (n-1,beta, p);
 new_p = x_add_y (n-1, new_r, Bp);
printf("\nnew_p[i]: \n");
for (i=0;i<n-1;i++) printf("%.2lf\t",new_p[i]);
printf("\n");

//while (error>0.01) {

 error = max_x (n-1,r);
printf("\nerror: %.2lf\t",error);
printf("\n");

/*free(p);
free(x);
free(r);*/
 p = new_p;
 x = new_x;
 r = new_r;
}

printf("\nx[i]:\n");
for (i=0;i<n-1;i++) printf("%.4lf\t",x[i]);
printf("\n");

/*printf("\nx[i]:\n");
for (l=0;l<n-1;l++) printf("%.2lf\t",x[l]);
printf("\nr[i]:\n");
for (l=0;l<n-1;l++) printf("%.2lf\t",r[l]);
printf("\np[i]:\n");
for (l=0;l<n-1;l++) printf("%.2lf\t",p[l]);
printf("\nerror: %.2lf",error);
printf("\n");*/

/*
free(p);
free(x);
free(r);
*/

/*printf("\nnew_x[i]:\n");
for (l=0;l<n-1;l++) printf("%.2lf\t",new_x[l]);
printf("\n");
printf("\nnew_r[i]:\n");
for (l=0;l<n-1;l++) printf("%.2lf\t",new_r[l]);
printf("\n");
printf("\nnew_p[i]:\n");
for (l=0;l<n-1;l++) printf("%.2lf\t",new_p[l]);
printf("\n");*/

 //for (i=0;i<n-1;i++) if (i==(n/2)) printf("%.3le\n",new_x[i]);
 /*for (i=0;i<n-1;i++) {
  sum=0.0;
  m_diff=0.0;
  for (j=0;j<n-1;j++) {
   if (i!=j) sum +=a[i][j]*x[j];}
  new_x[i]=(b[i]-sum)/2.0;
  t_diff=fabs(new_x[i]-x[i]);
  if (t_diff>m_diff) m_diff=t_diff;
  //if (i==(n/2)) printf("%.3le\n",new_x[i]);
  //if (i==(n/2)) printf("%d\t%.3le\t%.3lf\n",++k,m_diff,x[i]);
  //printf("%.3le\t",new_x[i]);
  x[i]=new_x[i];
 }
 k++;
 //printf("\n");*/
//} 
return 0;
}

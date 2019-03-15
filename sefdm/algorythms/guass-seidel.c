/*

A program to approximate the solution to of a system of equations,
using the Gauss-Seidel method. The system describes a network of
conductances connected at one of two nodes, to a source, and at 
the other a drain. 

./a

--  non-working  --

*/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>
#define max_its  10

int main(int argc,char *argv[])
{
 int i,j;
 int n;
 n=atoi(argv[1]);
 double a[n][n];
 double left,right;
 double max_b,b[n],x[n],x_k[n];
 double error=10.0;
 for (i=0;i<n;i++) {
  sum=0.0;
  for (j=0;j<n;j++) {
   //string of resistors VS->1->2->3->..n->0
   if (i==(j-1)||i==(j+1)) a[i][j]=1.0;
   else if (i!=j) a[i][j]=0.0;
   else a[i][j]=0.0;
   sum+=a[i][j];
  }
  a[i][i]=sum;
  x[i]=1.0;
  x_k[i]=1.0;
  if (i==0) max_b=b[i]=10.0;//Voltage at source
  else b[i]=0.0;
 }
 x[0]=10.0;	// voltage at end
 x_k[0]=10.0;
 x[(n-1)]=0.0;	// ground at end
 x_k[n-1]=0.0;
 sum=0.0;
 double t_diff,max_diff;
 clock_t start=clock(),diff;
 int iter=0;
 while (error>0.0000001) {//DBL_MIN) {
  iter++;
  max_diff=0.0;		// error
  t_diff=0.0;
  for (i=0;i<n;i++) { 	// find all x(k+1)[i]..x_k
   right=0.0;
   left=x[i];
   for (j=0;j<n;j++) {
     if (i<j) right+=a[i][j]*x[j];
     else if (i>j) left+=a[i][j]*x_k[j];
     else if (i==j){;}
   }
   x_k[i]=(b[i]-left-right)/a[i][i];
   t_diff=fabs(x[i]-x_k[i]);
   if (i!=0&&i!=(n-1)) {x[i]=x_k[i];	// assign new x
   if (t_diff>max_diff) max_diff=t_diff;}
  }
  error=max_diff/max_b;
 }
 diff=clock()-start;
 int msec=diff*1000/CLOCKS_PER_SEC;
 printf("%d\t%d\t%d.%d\n",n,iter,msec/1000,msec%1000);
 fflush(stdout);
 return 0;
}


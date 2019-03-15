/*
A program to approximate the solution to of a system of equations,
using successive over-relaxation and the openmp library. The system
describes a network of conductances connected at one of two nodes, 
to a source, and at the other a drain. 
./a [dimension]
$: dimension		number of iterations	ms
*/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#define max_its  10

int main(int argc,char *argv[])
{
 int i,j;
 int n;
 n=atoi(argv[1]);
 double a[n][n];
 double sum;
 double max_b,b[n],x[n],x_k[n],k_x[n];
 #pragma omp for
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
  k_x[i]=2.0;
  if (i==0) max_b=b[i]=10.0;//Voltage at source
  else b[i]=0.0;
 }
 x[0]=10.0;	// voltage at end
 x_k[0]=10.0;
 k_x[0]=10.0;
 x[(n-1)]=0.0;	// ground at end
 x_k[n-1]=0.0;
 k_x[n-1]=0.0;
 sum=0.0;
 double error=10.0;
 double w=2.0/3.0;
 double t_diff,max_diff;
 clock_t start=clock(),diff;
 int iter=0;
 while (error>0.000000001) {//DBL_MIN) {
  iter++;
  max_diff=0.0;		// error
  t_diff=0.0;
  #pragma omp for
  for (i=0;i<n;i++) { 	// find all x(k+1)[i]..x_k
   sum=0.0;
   for (j=0;j<n;j++) {
     if (i>j) sum+=a[i][j]*x[j];
     else if (i<j) {sum+=a[i][j]*k_x[j];}
     else if (i==j){;}
   }
   
   if (i!=(n-1)&&i!=0) {
    x_k[i]=(w*(b[i]-sum)/a[i][i])+((1-w)*x[i]);
    k_x[i]=x[i];
    x[i]=x_k[i];
   }
   t_diff=fabs(k_x[i]-x[i]);
   if (t_diff>max_diff) max_diff=t_diff;//}
  }
  error=max_diff/max_b;
  //printf("%d\t%.3le\t%.3le\n",iter,x[(n/2)],error);
  //fflush(stdout);
 }
 diff=clock()-start;
 int msec=diff*1000/CLOCKS_PER_SEC;
 printf("%d\t%d\t%d.%d\n",n,iter,msec/1000,msec%1000);
 fflush(stdout);
 return 0;
}

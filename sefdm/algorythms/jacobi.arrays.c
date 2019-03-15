#include <stdio.h>
#include <math.h>
#include <float.h>
//#define n 101
#include <time.h>
#include <stdlib.h>
#define max_its  10


int main(int argc,char *argv[])
{
 int i,j,k;
 int n;
 n=atoi(argv[1]);
 double a[n][n];
 double sum;
 double max_b,b[n],x[n],x_k[n];
 double error=10.0;
 for (i=0;i<n;i++) {
  sum=0.0;
  for (j=0;j<n;j++) {
   //string of resistors VS->1->2->3->..n->0
   if (i==(j-1)||i==(j+1)) a[i][j]=1/1.0;
   else if (i!=j) a[i][j]=0.0;
   else a[i][j]=0.0;
   sum+=a[i][j]; // sum of each row in the diagonal
  }
  a[i][i]=sum;
  x[i]=1.0;
  x_k[i]=1.0;
  if (i==0) max_b=b[i]=10.0;//Voltage at source
  else b[i]=0.0;
 }
 x[0]=10;
 x[(n-1)]=0.0;
 /*printf("\ni\tb[i]:\t\tx[i]:\t\tA:\n");
 fflush(stdout);
 for (i=0;i<n;i++) {
  printf("%d\t%.3le\t%.3le\t",i,b[i],x[i]);
  fflush(stdout);
  for (j=0;j<n;j++) {
   printf("%.3le\t",a[i][j]);
 fflush(stdout);
  }
  printf("\n");
 }*/
// printf("\n\n");
// printf("iter\tx[%d]\terror\n",(n/2));
// fflush(stdout);
 clock_t start=clock(),diff;
 double t_diff,max_diff;
 int iter=0;
 k=0;
 while (error>0.000000001) {//DBL_MIN) {
  iter++;
  max_diff=0.0;		// error
  t_diff=0.0;
  for (i=0;i<n;i++) { 	// find all x(k+1)[i]..x_k
 // in each row
   sum=0.0;
   for (j=0;j<n;j++) {
    if (i!=j) sum+=a[i][j]*x[j]; }
   x_k[i]=(b[i]-sum)/a[i][i];
   t_diff=fabs(x[i]-x_k[i]);
   if (i!=0&&i!=(n-1)) {
    x[i]=x_k[i];	// assign new x
    if (t_diff>max_diff) max_diff=t_diff;
   }
//   if (i==(n/2)) printf("%.3le\t",x[i]);
  }
//  printf("%d\t%.30lf\t",k++,x[n/2]);
//  fflush(stdout);
  error=max_diff/max_b;
//  printf("%.3le\n",error);
  fflush(stdout);
 }
 diff=clock()-start;
 int msec=diff*1000/CLOCKS_PER_SEC;
 printf("%d\t%d\t%d.%d\n",n,iter,msec/1000,msec%1000);
 fflush(stdout);
 return 0;
}

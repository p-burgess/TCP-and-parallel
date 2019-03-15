#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>
#define max_its  10

int main(int argc,char *argv[])
{
 int i,iter,j;//,k;
 int n;
 n=atoi(argv[1]);
 double a[n][n];
 double sum;//,sum_k;
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
     // sum of each row in the diagonal
  }
  a[i][i]=sum;
  x[i]=1.0;
  x_k[i]=1.0;
  if (i==0) max_b=b[i]=10.0;//Voltage at source
  else b[i]=0.0;
 }
 //}
 // normalize A:
 /*double l[n][n];
 for (i=0;i<n;i++) {
  //l[i][i]=1/a[i][i];
  for (j=0;j<n;j++) {
   if (i!=j) l[i][j]=a[i][j]/a[i][i];
   if (j>i) l[i][j]=0.0;
  }
  l[i][i]=1;
 }*/
 // print matrix
 /*printf("\n\tL:\n");
 for (i=0;i<n;i++) {
  for (j=0;j<n;j++) {
   printf("%.3le\t",l[i][j]);
   fflush(stdout);
  }
  printf("\n");
  fflush(stdout);
 }*/
 x[0]=10.0;	// voltage at end
 x_k[0]=10.0;
 x[(n-1)]=0.0;	// ground at end
 x_k[n-1]=0.0;
 //k=0;
 sum=0.0;
 //printf("\n");
 //printf("iter\tx[%d]\t\terror\n",(n/2));
 //fflush(stdout);
 //double divisor;
 double t_diff,max_diff;
 clock_t start=clock(),diff;
 while (error>0.0000001) {
  max_diff=0.0;		// error
  t_diff=0.0;
  for (i=0;i<n;i++) { 	// find all x(k+1)[i]..x_k
   sum=0.0;
   for (j=0;j<n;j++) {
     if (i<j) sum+=a[i][j]*x[j];
     else if (i>j) {sum+=a[i][j]*x_k[j];}
     else if (i==j){;}
   }
  if (i!=0&&i!=(n-1)) {
   x_k[i]=(b[i]-sum)/a[i][i];
  } else {
   x[i]=x_k[i];	// assign new x
  }
  t_diff=fabs(x[i]-x_k[i]);
  if (i!=0&&i!=(n-1)) {x[i]=x_k[i];	// assign new x
  if (t_diff>max_diff) max_diff=t_diff;}
  }
  error=max_diff/max_b;
 iter++;
 }
 /*printf ("\n x[i]: \n");
 for (i=0;i<n;i++) printf ("%.2lf  ",x[i]);
 printf ("\n");*/
 diff=clock()-start;
 int msec=diff*1000/CLOCKS_PER_SEC;
 printf("i%d\t%d\t%d.%d\n",n,iter,msec/1000,msec%1000);
 fflush(stdout);
 return 0;
}

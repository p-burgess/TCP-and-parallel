#include <stdio.h>
#define n 3
int main() {
	const double m[n][n]={{-2.0,0.0,1.0}, {0.0,-2.0,1.0}, {2.0,1.0,-9.0}};
	const double b[n]={-5.0,-5.0,0.0};
	double x[n], next_x[n];
	double sum;
	const int MaxIts=30;
	int Its,i,j;

//# pragma omp parallel
//{
//	# pragma omp for lastprivate(x)
	for(i=0;i<n;i++) x[i]=1.0; /* initialise */
//}
		for(Its=1;Its<=MaxIts;Its++) {
			printf("Its=%d",Its);

# pragma omp parallel
{
	# pragma omp for firstprivate(x),lastprivate(x)
			for(i=0; i<n; i++) {
				sum=0.0;
		   	for(j=0;j<n;j++) {
					if(i!=j)sum+=m[i][j]*x[j];
				}
				x[i]=(b[i]-sum)/m[i][i];
			}
}
			for(i=0;i<n;i++) {
				x[i]=next_x[i];
			   printf(" x[%d]=%12.8f",i,x[i]);
			}
			printf("\n");
		}
		return 0;
}

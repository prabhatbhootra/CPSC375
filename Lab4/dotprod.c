/* Ted Tierney
 * Prabhat Bhootra 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main(){
	int n;
	int i;
	clock_t start;
    	clock_t end;
	double dotprod = 0;
        printf("Enter n value: ");
	scanf("%d", &n);
	start = clock();
	double* x = (double*) malloc(n * sizeof(double));
	double* y = (double*) malloc(n * sizeof(double));
	for (i = 0; i < n; i++){
		x[i] = 1;
		y[i] = 1;
		dotprod += x[i]*y[i];
	}
	end = clock();
	printf("%f\n", dotprod);
	printf("Elapsed time = %f seconds\n", (float) (end - start)/CLOCKS_PER_SEC);
}

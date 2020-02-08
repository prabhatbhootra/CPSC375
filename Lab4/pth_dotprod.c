/* Ted Tierney
 * Prabhat Bhootra 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
int n;
double* x;
double* y;
double* bin;
void *dotprod(void* rank);    /* Thread function */

int main(){
	
	int i;
	int j;
	double final = 0.0;
	printf("Enter thread count value: ");
	scanf("%d", &thread_count);
	long thread;
	pthread_t* thread_handles;
	thread_handles = malloc(thread_count*sizeof(pthread_t));
	bin = (double*) malloc(thread_count * sizeof(double));

        printf("Enter n value: ");
	scanf("%d", &n);

	x = (double*) malloc(n * sizeof(double));
	y = (double*) malloc(n * sizeof(double));
	for (i = 0; i < n; i++){
		x[i] = 1;
		y[i] = 1;
	}
	
	clock_t start, end;
	start = clock();
	
	for (thread = 0; thread < thread_count; thread++)  
      		pthread_create(&thread_handles[thread], NULL, dotprod, (void*) thread);
	
	for (thread = 0; thread < thread_count; thread++) 
		pthread_join(thread_handles[thread], NULL); 

	free(thread_handles);
	printf("%f\n", bin[0]);

	end = clock();

	printf("Elapsed time = %f seconds\n", (float) (end - start)/CLOCKS_PER_SEC);
	return 0;
}

void *dotprod(void* rank) {
	long my_rank = (long) rank;
	double my_prod = 0.0;
	int a;
	int my_first_row;
	int my_last_row;
	int div = 2;
	int var = 1;
	int local_n = n/thread_count;
	if (my_rank < (n%thread_count)){
		my_first_row = my_rank*(local_n + 1);
		my_last_row = ((my_rank + 1)*(local_n + 1)) - 1;
	}
	else {
		my_first_row = my_rank*local_n;
		my_last_row = ((my_rank + 1)*local_n) - 1;	
	}
	for (a = my_first_row; a <= my_last_row; a++){
		my_prod += x[a]*y[a];
	}
	bin[my_rank] = my_prod;
	while (div <= thread_count){
		if (my_rank % div == 0){
			bin[my_rank] += bin[my_rank + var];
		}
		div *= 2;
		var *= 2;
	}
	return NULL; 
}

/* Ted Tierney
 * Prabhat Bhootra 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
int n;
double* x;
double* y;
double* bin;
int counter = 0;
sem_t count_sem; /*Initialize to 1 */
sem_t barrier_sem; /* Initialize to 0 */
sem_t count_sem1; /*Initialize to 1 */
sem_t barrier_sem1; /* Initialize to 0 */
sem_t count_sem2; /*Initialize to 1 */
sem_t barrier_sem2; /* Initialize to 0 */
sem_t count_sem3; /*Initialize to 1 */
sem_t barrier_sem3; /* Initialize to 0 */

void *dotprod(void* rank);    /* Thread function */


int main(){
	
	int i;
	int j;
	sem_init(&count_sem, 0, 1);
	sem_init(&barrier_sem, 0, 0);
	sem_init(&count_sem1, 0, 1);
	sem_init(&barrier_sem1, 0, 0);
	sem_init(&count_sem2, 0, 1);
	sem_init(&barrier_sem2, 0, 0);
	sem_init(&count_sem3, 0, 1);
	sem_init(&barrier_sem3, 0, 0);
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
	// printf("%f\n%f\n", bin[0], bin[thread_count/2 - 1]);
	int r;
	for (r = 0; r < thread_count; r++){
		printf("%f\n", bin[r]);
	}
	end = clock();

	printf("Elapsed time = %f seconds\n", (float) (end - start)/CLOCKS_PER_SEC);
	return 0;
}

void *dotprod(void* rank) {
	long my_rank = (long) rank;
	double my_prod = 0.0;
	int a;
	int h;
	int cut;
	int my_first_row;
	int my_last_row;
	int mult = 2;
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

	/* barrier 	*/
	sem_wait(&count_sem);
	if (counter == (thread_count - 1)){
		counter = 0;
		sem_post(&count_sem);	
		for (h = 0; h < thread_count - 1; h++){
			sem_post(&barrier_sem);
		}
	} else {
		counter++;
		sem_post(&count_sem);
		sem_wait(&barrier_sem);
	}

	int k;
	for (k = 0; k <= thread_count-1; k++){
		if (my_rank == k){
			if (my_rank % 2 == 0){
				if ((my_rank + 1) <= thread_count - 1){
					bin[my_rank] += bin[my_rank + 1];
				}
			}	
		}
	}
	
	/*barrier 1 */
	sem_wait(&count_sem1);
	if (counter == (thread_count - 1)){
		counter = 0;
		sem_post(&count_sem1);	
		for (h = 0; h < thread_count - 1; h++){
			sem_post(&barrier_sem1);
		}
	} else {
		counter++;
		sem_post(&count_sem1);
		sem_wait(&barrier_sem1);
	}

	while (div < thread_count - 1){	
		/*
		int s;
		if (thread_count % 2 != 0){
			cut = thread_count - 2;
		}
		else {
			cut = thread_count - 1;
		}
		*/
		if (my_rank % div == 0){
			if (my_rank + div < thread_count - 1){
				bin[my_rank] += bin[my_rank + div];
			}
		}

		/*
		for (s = 0; s <= cut; s++){
			if (s % div == 0){
				if (my_rank == s){
					if (my_rank + div < thread_count - 1){
						bin[my_rank] += bin[my_rank + div];
					}
				}
			}
		}
		*/
		div *= 2;
	}

	/* barrier 2 */
	sem_wait(&count_sem2);
	if (counter == (thread_count - 1)){
		counter = 0;
		sem_post(&count_sem2);	
		for (h = 0; h < thread_count - 1; h++){
			sem_post(&barrier_sem2);
		}
	} else {
		counter++;
		sem_post(&count_sem2);
		sem_wait(&barrier_sem2);
	}	
	
	if (thread_count % 2 != 0) {
		if (my_rank == thread_count - 1){
			bin[0] += bin[thread_count - 1];
			printf("HEREEEEEEEEEE\n");
		}	
	}

	/* barrier 3 */
	sem_wait(&count_sem3);
	if (counter == (thread_count - 1)){
		counter = 0;
		sem_post(&count_sem3);	
		for (h = 0; h < thread_count - 1; h++){
			sem_post(&barrier_sem3);
		}
	} else {
		counter++;
		sem_post(&count_sem3);
		sem_wait(&barrier_sem3);
	}	

	return NULL; 
}

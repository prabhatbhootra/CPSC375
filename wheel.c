/* CPSC 375 - Wheel of Primes
 * Seb Kryspin
 * Fumihiro Tamada
 * Prabhat Bhootra
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


void readInput(int N);

void *eliminate2();

void *eliminate3();

void *eliminate5();

int b0[5];
int b1[5];
int b2[5]; 

sem_t A;
sem_t B;
sem_t C;
sem_t D;
sem_t E;
sem_t F;



int main(int argc, char* argv[]) {
	pthread_t* thread1 = malloc(sizeof(pthread_t));
	pthread_t* thread2 = malloc(sizeof(pthread_t));
	pthread_t* thread3 = malloc(sizeof(pthread_t)); 

	int N = atoi(argv[1]); //read the input into N


	sem_init(&A, 0, 5);
	sem_init(&C, 0, 5);
	sem_init(&E, 0, 5); // create semaphors A, C, E initialized to 5
	
	sem_init(&B, 0, 0);
	sem_init(&D, 0, 0);
	sem_init(&F, 0, 0); // create semaphors B, D, F initilize to 0

	//Create threads 1, 2, 3
	pthread_create(thread1, NULL, eliminate2, (void*) 1);
	pthread_create(thread2, NULL, eliminate3, (void*) 2);
	pthread_create(thread3, NULL, eliminate5, (void*) 3); 

	readInput(N); 

	//join all threads
	pthread_join(*thread1, NULL);
	pthread_join(*thread2, NULL); 
	pthread_join(*thread3, NULL); 
	
	//free all threads memory
	free(thread1);
	free(thread2);
	free(thread3);

	//destroy all semaphores
	sem_destroy(&A);
	sem_destroy(&B);
	sem_destroy(&C); 
	sem_destroy(&D); 
	sem_destroy(&E); 
	sem_destroy(&F); 
	

}


/*Read the input into buffer */
void readInput(int N) {
	int value = 2; 
	int p = 0;
	while(value <= N) {
		sem_wait(&A); 
		b0[p] = value; 
		p = (p + 1) % 5; 
		sem_post(&B); 
		value++;
	}
	sem_wait(&A);
	b0[p] = -1; 
	sem_post(&B);
}



/*Thread 1 - Eliminate divisible by 2*/
void *eliminate2(void* rank) {
	int pRead = 0;
	int pWrite = 0;
	int value; 

	while(1) {
		sem_wait(&B);
		value = b0[pRead];
		sem_post(&A);

		if ((value % 2) != 0) {
			sem_wait(&C);
			b1[pWrite] = value;
			sem_post(&D); 
			pWrite = (pWrite + 1) % 5; 
			if (value == -1) { break;}
		}
		pRead = (pRead+1) % 5; 
	}
}

/*Thread 2 -Eliminate divisible by 3 */
void *eliminate3(void* rank) {
	int pRead = 0;
	int pWrite = 0;
	int value; 

	while(1) {
		sem_wait(&D);
		value = b1[pRead];
		sem_post(&C);

		 if ((value % 3) != 0) {
			sem_wait(&E);
			b2[pWrite] = value;
			sem_post(&F); 
			pWrite = (pWrite + 1) % 5; 
			if (value == -1) {break;}
		}
		pRead = (pRead+1) % 5; 
	}
}

/*Thread 3 - Eliminate divisible by 5 and output */
void *eliminate5(void* semaphoreE, void* semaphoreF) {
	int pRead = 0;
	int value; 

	while(1) {
		sem_wait(&F);
		value = b2[pRead];
		sem_post(&E);

		if (value == -1) {
			break;
		}
		else if ((value % 5) != 0) {
			printf("%d\n", value);
		}
		pRead = (pRead+1)%5;
	}
}



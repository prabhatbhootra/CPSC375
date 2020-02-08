/*CPSC 375 - lab 2 exercise

Prabhat Bhootra
J.Lewis Nikuze

------------------------------------------------------------
Number of tosses | Threads | Time elapsed      | pi value   |
------------------------------------------------------------
10000000         |   1     | 0.202716 seconds  |3.142218    |           
------------------------------------------------------------
10000000         |   2     | 0.206884 seconds  |3.142145    |           
------------------------------------------------------------
10000000         |   4     | 0.207885 seconds  |3.140725    |        
------------------------------------------------------------
10000000         |   8     | 0.255021 seconds  |3.143744    |           
------------------------------------------------------------
10000000         |   16    | 0.232351 seconds  |3.141126    |           
------------------------------------------------------------
10000000         |   32    | 0.241625 seconds  |3.137293    |           
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> 

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;
long long int number_in_circle = 0;  
long long int number_of_tosses;
pthread_mutex_t count_mutex;

void Usage(char* prog_name);
void *piCalc(void* rank);    /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

   thread_handles = malloc (thread_count*sizeof(pthread_t)); 


   printf("Enter number of tosses: ");
   scanf("%lli", &number_of_tosses);

   clock_t start, end;
  
   start = clock();

   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL, piCalc, (void*) thread);  


   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

   float pi_estimate = 4*number_in_circle/((double) number_of_tosses);

   end = clock();

   printf("pi is %f \n", pi_estimate);
   printf("Elapsed time = %f seconds\n", (float) (end - start)/CLOCKS_PER_SEC);  

   free(thread_handles);
   pthread_mutex_destroy(&count_mutex);
   return 0;
} 

void *piCalc(void* rank) {
    long long toss;
    long my_rank = (long) rank;
    long long thread_share = number_of_tosses/thread_count;
    long int my_number_in_circle = 0;

    unsigned int seed = time(NULL);

    for(toss = 0; toss < thread_share; toss++){
       double x = -1+2*((float)rand_r(&seed))/RAND_MAX;
       double y = -1+2*((float)rand_r(&seed))/RAND_MAX;
       long double distance_squared = x*x + y*y;

       if (distance_squared <= 1) {
        my_number_in_circle++;
      }
    } 
    pthread_mutex_lock(&count_mutex);
    number_in_circle += my_number_in_circle;
    pthread_mutex_unlock(&count_mutex);
    return NULL; 
}

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */

/* File:    5.10.c
 * Prabhat Bhootra
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[]) {
   int thread_count, n;
   double start, finish;
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);

   start = omp_get_wtime();
#  pragma omp parallel num_threads(thread_count)
   {
      int i;
      double my_sum = 0.0;

      for(i = 0; i < n; i++) {
#        pragma omp atomic
         my_sum += sin(i);
      }
   }
   finish = omp_get_wtime();

   printf("Thread_count = %d, n = %d, Time = %e seconds\n",
         thread_count, n, finish-start);
   return 0;
}

/* File:    omp_trap3.c
 * Prabhat Bhootra
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n, int thread_count, int* iterationsArray);

int main(int argc, char* argv[]) {
   double  global_result = 0.0;  /* Store result in global_result */
   double  a, b;                 /* Left and right endpoints      */
   int     n;                    /* Total number of trapezoids    */
   int 	   z, y;
   int     thread_count;
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);
   int* iterations = malloc(sizeof(int)*n);
   global_result = Trap(a, b, n, thread_count, iterations); 

   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result);
   for (z = 1; z < n; z++) {
      printf("%d iteration of the for loop is assigned to thread %d\n", z, iterations[z]);
   }
   return 0;
}  /* main */

void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
}  /* Usage */

double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */

double Trap(double a, double b, int n, int thread_count, int* iterationsArray) {
   double  h, approx;
   int  i;

   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0; 
#  pragma omp parallel for num_threads(thread_count) \
      reduction(+: approx) schedule(runtime)
   for (i = 1; i <= n-1; i++) {
     approx += f(a + i*h);
     iterationsArray[i] = omp_get_thread_num();
   }
   approx = h*approx; 
   return approx;
}  /* Trap */

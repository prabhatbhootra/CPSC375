/*CPSC 375 - lab 2 exercise

Prabhat Bhootra
J.Lewis Nikuze
-----------------------------------------------
Number of tosses |  Time elapsed    | pi value |
-----------------------------------------------
600000           | 0.023249 seconds |3.143693  |           
-----------------------------------------------
10000000         | 0.204979 seconds |3.142256  |           
-----------------------------------------------

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void main()
{
   long long int number_of_tosses;
   printf("Enter number of tosses: ");
   scanf("%lli", &number_of_tosses);

    clock_t start;
    clock_t end;
  
    start = clock();

   /* do something */
    long long int number_in_circle = 0;
    long long int toss;

    for (toss = 0; toss < number_of_tosses; toss++) {
       double x = -1+2*((float)rand())/RAND_MAX;
       double y = -1+2*((float)rand())/RAND_MAX;
       long double distance_squared = x*x + y*y;
       if (distance_squared <= 1) {
        number_in_circle++;
      }
    }

   float pi_estimate = 4*number_in_circle/((double) number_of_tosses);

   /* done */
   end = clock();

   printf("pi is %f \n", pi_estimate);

   printf("Elapsed time = %f seconds\n", (float) (end - start)/CLOCKS_PER_SEC);
}

//mpi_monte.c
//will laroche && Prabhat Bhootra
//calculate pi in multiple threads using MPI
//
//Results:
//  Fixed Value for N - 100,000,000 (1e8)
//  Threads         Time            Value of Pi
//  1               3.415 s         3.141551
//  2               2.604 s         3.141789
//  4               1.350 s         3.142115
//  8               1.413 s         3.141846
//  16              1.353 s         3.142146
//  32              0.875 s         3.142171
//  64              0.676 s         3.139927
//  128             0.541 s         3.142282
//
//  Fixed Value for Threads - 32:
//  Tosses          Time            Value of Pi
//  1e6             0.272 s         3.122176
//  1e7             0.265 s         3.147392
//  1e8             0.865 s         3.142515
//  1e9             5.666 s         3.141563
//  1e10            53.646 s        3.141605
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

        double start = MPI_Wtime();

        MPI_Request     sendreq;
        MPI_Request     recvreq;
        MPI_Status      status;
        int             numprocs;       //p
        long long int   total_in_circle;
        int             my_id;
        long long int   num_tosses;
        int             namelen;
        char            processor_name[MPI_MAX_PROCESSOR_NAME];
        char            *sendbuf, *recvbuf;
                                                      
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
        MPI_Get_processor_name(processor_name, &namelen);
                                                                                                                   
        if (my_id == 0) {
	        num_tosses = atol(argv[1]);
        }
        // broadcast total number of tosses and threads to each thread
        // each thread then does a portion of the work based on the totals
        MPI_Bcast(&num_tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&numprocs, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int toss;
        long long int local_incircle;
        long long int local_tosses;
        local_tosses = num_tosses / numprocs;
        local_incircle = 0;
        if (my_id < (num_tosses % numprocs))    local_tosses += 1;
        unsigned int seed = time(NULL);
        // calculates local total in the circle for each thread
        for (toss = 0; toss < local_tosses; toss++) {
                double x = -1 + 2 * ((float)rand_r(&seed))/RAND_MAX;
                double y = -1 + 2 * ((float)rand_r(&seed))/RAND_MAX;
                long double d_sq = x*x + y*y;
                if (d_sq <= 1) {
	                local_incircle++;
                }
        }
        // reduces / sums each local total into the global total
        MPI_Reduce(&local_incircle, &total_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
        if (my_id == 0) {
                printf("pi - %f\n", 4 * (double)total_in_circle/((double)num_tosses));
                printf("elapsed time - %f\n", MPI_Wtime() - start);
        }
        // cleaning up
        MPI_Finalize();

        return 0;
}

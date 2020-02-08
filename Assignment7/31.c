#include <stdio.h>
#include <string.h>
#include <mpi.h>

 const int MAX STRING = 100; 
 int main(void) { 
 	char greeting[MAX STRING]; 
 	int comm sz; /∗ Number of processes ∗/
 	int my rank; /∗ My process rank ∗/
 	MPI Init(NULL, NULL); 
 	MPI Comm size(MPI COMM WORLD, &comm sz);
 	MPI Comm rank(MPI COMM WORLD, &my rank); 
 	if (my rank != 0) { 
 		sprintf(greeting, "Greetings from process %d of %d!", 18 my rank, comm sz); 
 		MPI Send(greeting, strlen(greeting)+1, MPI CHAR, 0, 0, 20 MPI COMM WORLD); 
 	} else { 
 		printf("Greetings from process %d of %d!\n", my rank, comm sz); 
 		for (int q = 1; q < comm sz; q++) { 
 			MPI Recv(greeting, MAX STRING, MPI CHAR, q, 25 0, MPI COMM WORLD, MPI STATUS IGNORE); 
 			printf("%s\n", greeting); 
 		} 
 	}
 	MPI Finalize();
 	return 0; 
 } /∗ main ∗/

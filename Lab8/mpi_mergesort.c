/*
 * Lab08 11/18/2019
 * Prabhat Bhootra - Fumihiro Tamada
 *
 * This program is a parallized merge sort algorithm
 *
 *	n = 10000000
 *
 *	number of process	|	time(s)
 *	---------------------------------------------
 *		1		|	0.000000
 *		2		| 	0.090210
 *		4		|	0.107325
 *		8		| 	2.224212
 *		16		|	1.458114
 *		32		|	3.627930
 *		64		| 	2.594803
 *		128		|	3.067386 	
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int comp (const void * a, const void * b){
	return ( *(int*)a - *(int*)b );
}

int merge(int* arr1,int *arr2,int *new_arr,int n){
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int i;
	while(p1<n && p2<n){
		if (arr1[p1] < arr2[p2]){
			new_arr[p3] = arr1[p1];
			p3++;
			p1++;
		}
		else{
			new_arr[p3] = arr2[p2];
			p3++;
			p2++;
		}
	}
	if(p1==n-1 && p2==n-1){
		return 0;
	}
	else if(p1<n-1){
		for(i=p1;i<n;i++){
			new_arr[p3] = arr1[i];
			p3++; 
		}
	}
	else{
		for(i=p2;i<n;i++){
			new_arr[p3] = arr2[i];
			p3++;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	MPI_Request	sendreq;
	MPI_Request	recvreq;
	MPI_Status	status;
	double 		start, end;
	int		numprocs;
	int		myid;
	int 		i;
	long long int	n;
	long long int 	local_n;
	int 		*arr;
	int 		*local_arr;
	int 		*temp_arr;
	int		*receive_arr;	

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if(myid == 0){
		n = atol(argv[1]);
		arr = (int*) malloc(n*sizeof(int));	
		local_n = n/numprocs;
	}
	
	MPI_Bcast(&local_n,1,MPI_LONG_LONG,0,MPI_COMM_WORLD);
	MPI_Bcast(&n,1,MPI_LONG_LONG,0,MPI_COMM_WORLD);
	local_arr = (int*) malloc(local_n*sizeof(int));
	
	if(myid == 0){
		for(i=0;i<n;i++){
			arr[i] = rand()%1000;
		}
		MPI_Scatter(arr,local_n,MPI_INT,local_arr,local_n,MPI_INT,0,MPI_COMM_WORLD);
	}
	else{

		MPI_Scatter(arr,local_n,MPI_INT,local_arr,local_n,MPI_INT,0,MPI_COMM_WORLD);
	}

	qsort(local_arr,local_n,sizeof(int),comp);		
		
	
	start = MPI_Wtime();
	i = 2;
	while (i <= numprocs) {
		if(myid%i != 0){
			MPI_Send(local_arr,local_n,MPI_INT,myid-i/2,0,MPI_COMM_WORLD);
			break;	
		}
		else{
			receive_arr = (int*) malloc(local_n*sizeof(int));
			temp_arr = (int*) malloc(2*local_n*sizeof(int));
			MPI_Recv(receive_arr,local_n,MPI_INT,myid+i/2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			merge(local_arr,receive_arr,temp_arr,local_n);
			local_arr = temp_arr;
		}
		i = i*2;
		local_n = local_n*2;
		
	}
	end = MPI_Wtime();

	if (myid == 0){
		for(i = 0;i<100;i++)
			printf("%d ",local_arr[i]);
		printf("\n");
		printf("The time elapsed is %f\n",end-start);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	MPI_Finalize();
		
}

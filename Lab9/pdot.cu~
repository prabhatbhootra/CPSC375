/* Prabhat Bhootra and Shelby Cass
 * 
 * Timing results:
 *	n   |   T   | time (ms)
 *      5   |   1   | 0.0     
 *      512 |   1   | 0.0
 *      512 |   4   | 0.0
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 512

__global__ void dot (int *a, int *b, int *c) {
	__shared__ int temp[N];
	temp[threadIdx.x] = a[threadIdx.x] * b[threadIdx.x];
 
	__syncthreads();
	
	if (0 == threadIdx.x) {
		int sum = 0;
		int i;
		for (i = 0; i < N; i++)
			sum += temp[i];
		*c = sum;
	
	}
}

int main (int argc, char *argv[]) {
	int x;
	int n = atoi(argv[1]);
	int T = atoi(argv[2]);
	int *a, *b;
	int c;
	int *dev_a, *dev_b, *dev_c;
	int size = N * sizeof(int);
	
	cudaMalloc ((void**) &dev_a, size);
	cudaMalloc ((void**) &dev_b, size);
	cudaMalloc ((void**) &dev_c, sizeof(int));

	a = (int *) malloc(size);
	b = (int *) malloc(size);

	for (x = 0; x < n; x++) {
		a[x] = 1;
		b[x] = 1;
	}

	cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	dot<<<T, N>>>(dev_a, dev_b, dev_c);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime,start, stop);
	printf("The elapsed time: %3.1f ms\n", elapsedTime);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cudaMemcpy(&c, dev_c, sizeof(int), cudaMemcpyDeviceToHost);
	printf("%d\n", c);

	free(a); free(b);
	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);

	return 0;	
}

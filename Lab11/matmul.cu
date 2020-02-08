#include <stdlib.h>
#include <stdio.h>

__global__ void MatrixMulKernel(float *M, float *N, float *P, int Width) {
	// Calculate the row index of the P element and M
	int Row = blockIdx.y*blockDim.y+ threadIdx.y;

	// Calculate the column index of P and N
	int Col = blockIdx.x*blockDim.x+ threadIdx.x;

	if ((Row < Width) && (Col < Width)) {
		float Pvalue= 0.0;
		// each thread computes one element of the block sub-matrix
		for (int k = 0; k < Width; ++k) {
			Pvalue += M[Row*Width+k]*N[k*Width+Col];
		}
		//printf("%f\n", Pvalue);
		P[Row*Width+Col] = Pvalue;
	}
}

int main (int argc, char *argv[]) {
	int i, j;
	int k = atoi(argv[1]);
	int *dev_k;
	float *m, *n, *p;
	float *dev_m, *dev_n, *dev_p;

	m = (float *) malloc(k*k*sizeof(float));
 	n = (float *) malloc(k*k*sizeof(float));
	p = (float *) malloc(k*k*sizeof(float));

	//malloc  k variable in device

	for (i = 0; i < k; i++) {
		for (j = 0; j < k; j++){
			m[i*k + j] = 1.0;
			n[i*k + j] = 1.0;
		}
	}
	cudaMalloc ((void**) &dev_m, k*k*sizeof(float));
	cudaMalloc ((void**) &dev_n, k*k*sizeof(float));
	cudaMalloc ((void**) &dev_p, k*k*sizeof(float));
	cudaMalloc ((void**) &dev_k, sizeof(int));

	cudaMemcpy(dev_m, m, k*k*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_n, n, k*k*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(&dev_k, k, sizeof(int), cudaMemcpyHostToDevice);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	dim3 DimGrid((k-1)/16 + 1, (k-1)/16+1, 1);
	dim3 DimBlock(16, 16, 1);
	MatrixMulKernel<<<DimGrid, DimBlock>>>(dev_m, dev_n, dev_p, dev_k);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime,start, stop);
	printf("The elapsed time: %3.1f ms\n", elapsedTime);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cudaMemcpy(p, dev_p, k*k*sizeof(float), cudaMemcpyDeviceToHost);

	for (i = 0; i < k; i++) {
		for (j = 0; j < k; j++){
			printf("%f\n", p[i*k + j]);
		}
	}
	
	free(m);
	free(n);
	free(p);
	cudaFree(dev_m);
	cudaFree(dev_n);
	cudaFree(dev_p);
}

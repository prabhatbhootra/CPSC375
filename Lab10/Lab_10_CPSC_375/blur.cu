/*
    Authors: Erkin Verbeek, Prabhat Bhootra
    Date: 12/2/2019

 *     *** with 3 x 3 patch ***
 *
	M = 10000
	N = 10000
	The elapsed time: 71.6 ms

    M = 30000
	N = 10000
	The elapsed time: 215.1 ms
	
	M = 20000
	N = 20000
	The elapsed time: 227.6 ms
	
 *     *** with 7 x 7 patch ***
 
	M = 10000
	N = 10000
	The elapsed time: 225.5 ms

	M = 30000
	N = 10000
	The elapsed time: 766.0 ms
 
	M = 20000
	N = 20000
	The elapsed time: 964.6 ms
	
	
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 255 // the max value for each pixel 
#define M 10000
#define N 10000

__global__ void blurKernel(unsigned char * in_img,
		unsigned char * out_img, int patchWidth) {
	
	int Col = blockIdx.x*blockDim.x + threadIdx.x;
	int Row = blockIdx.y*blockDim.y + threadIdx.y;
	
	if (Col < N && Row < M) {
		int pixVal = 0;
		int pixels = 0;
	
	
		for (int blurRow = -patchWidth; blurRow < patchWidth + 1; blurRow++) {
			for (int blurCol = -patchWidth; blurCol < patchWidth + 1; blurCol++) {
				int curRow = Row + blurRow;
				int curCol = Col + blurCol;
			
				if (curRow > -1 && curRow < M && curCol > -1 && curCol < N) {
					pixVal += in_img[curRow*N + curCol];
					pixels++;
				}
			}
		}
		out_img[Row*N + Col] = (unsigned char) (pixVal/pixels);
	}
	
}


int main(int argc, char* argv[]) {
	int i;
	unsigned char * img;	// original IMG
	unsigned char * dev_img;	// original IMG
	unsigned char * dev_newImg;	// new IMG
	int size = M * N * sizeof(unsigned char);
	
	// dimensions of the grid and the block
	dim3 DimGrid((N-1)/16 + 1, (M-1)/16+1, 1);
	dim3 DimBlock(16, 16, 1);
	
	// allocates memory on device
	cudaMalloc((void **)&dev_img, size);	// device space for original img
	cudaMalloc((void **)&dev_newImg, size);	// device space for new img
	
	// allocate memory on host
	img = (unsigned char * ) malloc(size);
	
	srand(time(0)); // seed the random number generator
  
  	// we iterate through img and fill with random values
  	for (i = 0; i < M * N; i++) {
		img[i] = rand() % (MAX_VAL + 1);
		//printf("img[%d] = %d\n", i, img[i]);
  	}
  	printf("M = %d\nN = %d\n", M, N);
	
	cudaMemcpy(dev_img, img, size, cudaMemcpyHostToDevice);	// copy image from host to device
	
	float elapsedTime;		// timing stuff
	cudaEvent_t start,end;
	cudaEventCreate(&start);
	cudaEventCreate(&end);
	cudaEventRecord(start,0);
	
	//launch dot() kernel with 1 block and N threads
	blurKernel<<<DimGrid, DimBlock>>>(dev_img, dev_newImg, 3);
	
	cudaEventRecord(end, 0);	// stop timer
	cudaEventSynchronize(end);
	cudaEventElapsedTime(&elapsedTime,start,end);
	
	// copy device result back to the host copy of c
	
	cudaMemcpy(img, dev_newImg, size, cudaMemcpyDeviceToHost);	// copy new image back to host
	
	/*
	// write out new image so we can see if we did anything
  	for (i = 0; i < M * N; i++)
    	printf("newImg[%d] = %d\n", i, img[i]);
	*/
	
	// print elapsed time
	printf("The elapsed time: %3.1f ms\n", elapsedTime);
	cudaEventDestroy(start);
	cudaEventDestroy(end);
	
	// free up heap space variables
	free(img);
	cudaFree(dev_img);
	cudaFree(dev_newImg);
	return 0;
}





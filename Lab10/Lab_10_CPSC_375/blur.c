/* Erkin Verbeek & Prabhat Bhootra
 * 12/2/2019
 *
 * Serial bluring of images
 *
 *   Results:
 *
 *
 *     *** with 3 x 3 patch ***
 *
 * int M = 10000;
 * int N = 10000;
 * Elapsed seconds = 4
 *
 * int M = 1000;
 * int N = 1000;
 * Elapsed seconds = 0
 *
 * int M = 30000;
 * int N = 10000;
 * Elapsed seconds = 11
 *
 * 
 *      *** with 7 x 7 patch ***
 *
 * int M = 10000;
 * int N = 10000;
 * Elapsed seconds = 18
 * 
 * int M = 1000;
 * int N = 1000;
 * Elapsed seconds = 0
 * 
 * int M = 30000;
 * int N = 10000;
 * Elapsed seconds = 53
 * 
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 255 // the max value for each pixel 

// function prototype
unsigned char * blurImg(unsigned char * img, int M, int N, int patchWidth);

int main(int argc, char* argv[]) {
  int i;
  time_t start_time, end_time;
  int M = 30000;
  int N = 10000;
  
  unsigned char * img = malloc(M * N * sizeof(unsigned char)); // declares char array
  
  srand(time(0)); // seed the random number generator
  
  // we iterate through img and fill with random values
  for (i = 0; i < M * N; i++) {
    img[i] = rand() % (MAX_VAL + 1);
    //printf("img[%d] = %d\n", i, img[i]);
  }
  
  
  unsigned char * newImg;
  
  // start time
  start_time = time(NULL);
  
  // blur the image with given patch width
  newImg = blurImg(img, M, N, 3);  

  // end time
  end_time = time(NULL);
  
  /*
  // write out new image so we can see if we did anything
  for (i = 0; i < M * N; i++)
    printf("newImg[%d] = %d\n", i, newImg[i]);
  */
  
  
  // print out time taken
  printf("Elapsed seconds = %ld\n", (end_time - start_time));
  free(img);
  free(newImg);
}



unsigned char * blurImg(unsigned char * img, int M, int N, int blurSize) {
  int rowIdx, colIdx;
  
  // initialize a new return img
  unsigned char * retImg = malloc(M * N * sizeof(unsigned char));
  
  // iterates along every pixel in the img
  for (rowIdx = 0; rowIdx < M; rowIdx++) {
    for (colIdx = 0; colIdx < N; colIdx++) {
      
      // initialize pixel variables
      int pixVal = 0;
      int pixels = 0; // number of pixels
      
      // iterate along all the pixels in the patch around the current pixel
      int blurRow, blurCol;
      for (blurRow = -blurSize; blurRow < blurSize + 1; blurRow++) {
        for (blurCol = -blurSize; blurCol < blurSize + 1; blurCol++) {
          
          int curRow = rowIdx + blurRow;
          int curCol = colIdx + blurCol;
          // verify we have a valid image pixel
          if (curRow > -1 && curRow < M && curCol > -1 && curCol < N) {
            pixVal += img[curRow * N + curCol]; // add pixels value to pixVal
            pixels++; // increment number of pixels hit
          }
        }
      }
      
      // write the new pixel value to the output image
      retImg[rowIdx * N + colIdx] = (unsigned char) (pixVal / pixels);
    }
  }
  return retImg; // overwrite image with new created image
}







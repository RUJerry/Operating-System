#include <stdio.h>
//#include "analyzecache.h"
#include <time.h>
#include <stdlib.h>

#define SIZE 16*1024*1024
#define ARR_SIZE 64*1024*1024

void cacheLineSizeTest(int arr[]){
    double elapsedTime;
    int i, k;
    clock_t start, finish;
    
    for(k = 1; k <= 1024; k <<= 1) {
        start = clock();
        for (i=0; i<ARR_SIZE; i += k) {
            arr[i] *= 3;
        }
        finish = clock();
        
        elapsedTime = (double)(finish - start)/CLOCKS_PER_SEC;
        printf("%d %1.5f \n", k, elapsedTime);
    }
}


int main(int argc, char *argv[])
{
    static int arr[ARR_SIZE];
    cacheLineSizeTest(arr);
    int cacheLineSize = 32;
    printf("Cache Block/Line Size: %d B", cacheLineSize);
    return 0;
}



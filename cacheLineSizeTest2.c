#include <stdio.h>
//#include "analyzecache.h"
#include <time.h>
#include <stdlib.h>


#define KB 1024
#define MB 1024*1024

#define SIZE 16*KB
#define ARR_SIZE 64*MB

int getJumpPoint(double steps[]){
    double pre = steps[0];
    int flat = 0;
    int i=1;
    for (i=1; i<13; i++) {
        if (flat && steps[i]/pre < 0.85 )
            return 1<<(i-1);
        if (steps[i]/pre > 0.85 )
            flat = 1;
        pre = steps[i];
    }
    return 0;
}

// Function to get the cache line size.
// Return integer in power of 2.
int cacheLineSizeTest(char arr[]){
    double elapsedTime;
    int i, k;
    int j=0;
    
    double steps[13];
    clock_t start, finish;
    
    for (i=0; i<ARR_SIZE; i++) {
        arr[i] = 'a';
    }

    for(k = 1; k <= 4096; k <<= 1) {
        start = clock();
        for (i=0; i<ARR_SIZE; i += k) {
            arr[i] = 'b';
        }
        finish = clock();
        elapsedTime = (double)(finish - start)/CLOCKS_PER_SEC;
        steps[j++] = elapsedTime;
        printf("steps: %5d run time: %8.5f \n", k, elapsedTime);
    }
    int jumpPoint = getJumpPoint(steps);
    return jumpPoint;
}

// Function to get the element with maximum occurrence in an array.
int maxCount(int a[]){
    int max = 0;
    int index = 0;
    int i;
    for (i=0; i<10; i++) {
        int cur = a[i];
        int curCount = 1;
        int j;
        for (j=i+1; j<10; j++) {
            if (a[j] == cur)
                curCount++;
        }
        if(curCount>max){
            max = curCount;
            index = i;
        }
    }
    return a[index];
}

int getCacheLineSize(char arr[]){
    int sizes[10];
    // Here we run 10 times cacheLineTest to get 10 results of cache line size
    // Then we take the one with maximum occurrence.
    int i;
    for (i=0; i<10; i++) {
        printf("Test %d: \n", i);
        sizes[i] = cacheLineSizeTest(arr);
    }
    return maxCount(sizes);
}

int main(int argc, char *argv[])
{
    static char arr[ARR_SIZE];
    
    int cacheLineSize = getCacheLineSize(arr);
    printf("Cache Block/Line Size: %d B ", cacheLineSize);
    return 0;
}



#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define KB 1024
#define MB 1024*1024

#define SIZE 16*MB
#define ARR_SIZE 64*MB


//This method performs the cache hits
double cacheSize (int size, int arr[]){
    clock_t start;
    clock_t finish;
    int i,j;
    double elapsedTime;
    //block of code to fetch the array into memory
    //once before the cache verification code is executed.
    for(i=0;i<size;i++) {
        arr[i] += 1;
    }
    start = clock();
    for( j=0; j < ARR_SIZE ; j++){
        arr[(j*32) % (size)] += 1;
    }
    finish = clock();
    elapsedTime = (double)(finish - start)/CLOCKS_PER_SEC;
    srand(time(NULL));
    
    //block of code to indicate to compiler that random stuff
    //is being done to the array so that it does not try and optimize above operations
    int x = (rand() % size);
    arr[size-x] = 4;
    return elapsedTime;
}

int cacheJumpPoint(double steps[]){
    double pre = steps[0];
    int i;
    for (i=1; i<15; i++) {
        printf("ratio: %6.6f \n", steps[i]/pre);
        //*1 when the current-previous ratio is greater than 1.25, we record previous as jump point.
        if (steps[i]/pre > 1.25) {
            return 1<<(i-3);
        }
        
        //*2 if the ratio is between 0.8 and 1.2, we just regard it as flat and continue
        if (0.8 < steps[i]/pre || steps[i]/pre < 1.2) {
            pre = steps[i];
            continue;
        }
                break;
    }
    // No valid jump point found, return -1.
    return -1;
}

int cacheSizeTest(int arr[]){
    int i;
    double elapsedTime;
    double steps[15];
    int j = 0;
    for( i = 1024; i <= SIZE; i <<= 1) {
        elapsedTime = cacheSize(i,arr);
        steps[j++] = elapsedTime;
        printf("Size: %5d KB, %6.6f \n", i/1024 , elapsedTime);
    }
    int jumpPoint = cacheJumpPoint(steps);
    return jumpPoint;
}

// Function to get the element with maximum occurrence in an array.
int maxCount(int a[], int n){
    int max = 0;
    int index = 0;
    int i;
    for (i=0; i<n; i++) {
        int cur = a[i];
        int curCount = 1;
        int j;
        for (j=i+1; j<n; j++) {
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

int getJumpPoint(double steps[]){
    double pre = steps[0];
    int flat = 0;
    int i=1;
    for (i=1; i<13; i++) {
        if (flat && steps[i]/pre < 0.85 )
            return 1<<(i-2);
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

//*******************************************************
//*****      Calculate Cache Line Size            *******
//*******************************************************
int getCacheLineSize(char arr[]){
    int n = 20;
    int sizes[n];
    // Here we run n times cacheLineTest to get 10 results of cache line size
    // Then we take the one with maximum occurrence.
    int i;
    for (i=0; i<n; i++) {
        printf("Test %d: \n", i);
        sizes[i] = cacheLineSizeTest(arr);
    }
    return maxCount(sizes, n);
}

//*******************************************************
//*****      Calculate L2 Cache Size              *******
//*******************************************************

int getCacheSize(int arr[]){
    int n = 5;
    int sizes[n];
    int i;
    for (i=0; i < n; i++) {
        printf("Test %d: \n", i);
        sizes[i] = cacheSizeTest(arr);
    }
    return maxCount(sizes, n);
}

//*******************************************************
//*****      Calculate the Cache Miss Penalty     *******
//*******************************************************
double cacheMissPenaltyTest(int cacheSize, char arr[]){
    double elapsedTime;
    double penalty;
    int i;
    
    clock_t start, finish;
    for (i = 0 ; i<ARR_SIZE; i++) {
        arr[i] = 'a';
    }
    
    start = clock();
    for (i = 0 ; i<ARR_SIZE; i += 2*cacheSize*KB) {
        arr[i] = 'b';
    }
    finish = clock();
    elapsedTime = 1000*1000*(double)(finish - start)/CLOCKS_PER_SEC;
    
    penalty = elapsedTime/(ARR_SIZE / (2*cacheSize*KB) );
    printf("time: %8.8f gaps: %3d Cache miss penalty: %6.8f us \n",elapsedTime, ARR_SIZE / (2*cacheSize*KB), penalty);
    
    
    //srand(time(NULL));
    //int x = (rand() % cacheSize);
    //arr[cacheSize-x] = 4;
    return penalty;
}


double cacheMissPenalty(int cacheSize, char arr[]){
    int i;
    int n = 10;
    double result;
    for (i=0; i<n; i++) {
        result += cacheMissPenaltyTest(cacheSize, arr);
    }
    return 0.1*result;
}

int main(int argc, char *argv[]){
    static int arr[ARR_SIZE];
    static char arrc[ARR_SIZE];
    
    //int cacheLineSize = getCacheLineSize(arrc);
    //int cacheSize = getCacheSize(arr);
    double penalty = cacheMissPenalty(256, arrc);
    
    //printf("Cache Block/Line Size: %d B \n", cacheLineSize);
    //printf("Cache size: %d KB \n", cacheSize);
    printf("Cache Miss Penalty: %6.8f us \n", penalty);
    return 0;
}



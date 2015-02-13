//
//  main.c
//  cacheSizeTest
//
//  Created by Junjie Huang on 2/13/15.
//  Copyright (c) 2015 Junjie Huang. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define KB 1024
#define MB 1024*1024

#define SIZE 16*MB
#define ARR_SIZE 64*MB


/*This method performs the cache hits*/
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
        if (steps[i]/pre > 1.25) {
            return 1<<(i-1);
        }
        if (0.8 < steps[i]/pre || steps[i]/pre < 1.2) {
            pre = steps[i];
            continue;
        }
                break;
    }
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



int main(int argc, char *argv[])
{
    static int arr[ARR_SIZE];
    int re = getCacheSize(arr);
    printf("Cache size: %d \n", re);
    
    return 0;
}



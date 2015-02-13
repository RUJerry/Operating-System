#include <stdio.h>
#include "analyzecache.h"
#include <time.h>
#include <stdlib.h>

#define SIZE 16*1024*1024
#define ARR_SIZE 64*1024*1024

/*This method performs the cache hits*/
	double cacheTest (int size, int arr[]){
	clock_t start;
	clock_t finish;
	int i,j;
	double elapsedTime;
	/*block of code to fetch the array into memory
	once before the cache verification code is executed.*/
	for(i=0;i<size;i++) {
		arr[i] +=  size%32;
	}
	start = clock();
	for( j=0; j < ARR_SIZE ; j++){
		arr[(j*32) % (size)]+=16;
	}
	finish = clock();
	elapsedTime = (double)(finish - start)/CLOCKS_PER_SEC;
	srand(time(NULL));
	
	/*block of code to indicate to compiler that random stuff
	is being done to the array so that it does not try and optimize 
	above operations*/
	int x = (rand() % size);
	arr[size-x] = 4;
	return elapsedTime;
}


int main(int argc, char *argv[])
{
	static int arr[ARR_SIZE];
	int i;
	double elapsedTime;
	for( i = 1024; i <= SIZE; i <<= 1) {
		elapsedTime = cacheTest(i,arr);
		printf("%d, %1.2f \n", i/1024 , elapsedTime);
	}
	return 0;
}



#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

#define N 1000000
int counts = N;

void handle_signal(int signum)
{
	--counts;
	__asm__ ("pop %eax\n\t");
	__asm__ ("pop %ecx\n\t");
	__asm__ ("pop %edx\n\t");
	__asm__ ("push %ecx\n\t");
	__asm__ ("push %eax\n\t");
	//printf(1,"in handle\n");
	if(!counts){
		__asm__ ("movl $0x6e, 8(%ebp)\n\t");
	}
	
}

int main(int argc, char *argv[])
{
	signal(SIGFPE, handle_signal);
	
	int x = 5;
	int y = 0;

	int start = uptime();
	x = x / y;
	int end = uptime();
	
	int elapsedTime = 10000*(end - start);// time in micro-second
	int avTime = elapsedTime / N;
	
	printf(1, "Traps Performed: %d\n", N);
	printf(1, "Total Elapsed Time: %d\n", elapsedTime);
	printf(1, "Average Time Per Trap: %d\n", avTime);
	
	exit();
}

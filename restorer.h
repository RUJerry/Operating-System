// You must define an inline asm function here to solve stage3.
void restorer(void){
	//__asm__("movl %ebp, %esp");
	//printf(1,"in restorer\n");
	__asm__("pop %ebp");
	
	__asm__("pop %edx");
	__asm__("pop %edx");
	__asm__("pop %ecx");
	__asm__("pop %eax");
	
	//__asm__("push %ebp");
	__asm__("ret");
	//__asm__("movl $0x9b, 4(%ebp)");
	//__asm__("sub $4, %ebp");
	//__asm__("movl %esp,%ebp");
	//__asm__("pushl %ebp");
	//__asm__("movl $71, %ebp");
}

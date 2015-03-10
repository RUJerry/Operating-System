// You must define an inline asm function here to solve stage3.
void restorer(int r){
	//__asm__("movl %ebp, %esp");
	//printf(1,"restorer");
	__asm__("popl %ebp");
	
	__asm__("popl %edx");
	__asm__("popl %edx");
	__asm__("popl %ecx");
	__asm__("popl %eax");
	
	__asm__("pushl %ebp");
	//__asm__("movl %esp,%ebp");
	//__asm__("pushl %ebp");
	//__asm__("movl $71, %ebp");
}

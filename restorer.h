// You must define an inline asm function here to solve stage3.
void restorer(void){
	__asm__("movl %ebp, %esp");
	__asm__("popl %ebp");
	
	__asm__("popl %edx");
	__asm__("popl %edx");
	__asm__("popl %ecx");
	__asm__("popl %eax");
	
	__asm__("addl $16,%ebp");
	__asm__("pushl %ebp");
	__asm__("movel $4, %eip");
}

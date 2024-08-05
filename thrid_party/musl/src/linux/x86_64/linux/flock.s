.hidden __syscall_ret
.global flock
.type flock,@function
flock:
	pop %rdx
	movl $73,%eax //__NR_flock
	syscall
	push %rdx
	mov %rax,%rdi
	jmp __syscall_ret

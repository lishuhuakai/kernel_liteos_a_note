.text
.balign 16
.hidden __syscall_ret
.global flock
.type flock,%function
flock:
	mov x8,#32 //__NR_flock
	svc #0
	b __syscall_ret

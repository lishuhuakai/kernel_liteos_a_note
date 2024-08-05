.text
.balign 16
.hidden __syscall_ret
.global flock
.type flock,%function
flock:
	mov ip,r7
	mov r7,#143 //__NR_flock
	svc #0
	mov r7,ip
	b __syscall_ret

.ifndef __LITEOS__
.include "crtbrand.s"
.endif

.section .init
.global _init
.type _init,%function
.ifndef __LITEOS__
.balign 4
.endif
_init:
	stp x29,x30,[sp,-16]!
	mov x29,sp

.section .fini
.global _fini
.type _fini,%function
.ifndef __LITEOS__
.balign 4
.endif
_fini:
	stp x29,x30,[sp,-16]!
	mov x29,sp

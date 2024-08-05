.include "crtbrand.s"

.section .init
.global _init
.type _init,%function
.balign 4
_init:
	stp x29,x30,[sp,-16]!
	mov x29,sp

.section .fini
.global _fini
.type _fini,%function
.balign 4
_fini:
	stp x29,x30,[sp,-16]!
	mov x29,sp

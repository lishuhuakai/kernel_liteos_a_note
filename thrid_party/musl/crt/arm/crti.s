.ifndef __LITEOS__
.include "crtbrand.s"
.endif

.syntax unified

.section .init
.global _init
.type _init,%function
.ifndef __LITEOS__
.balign 4
.endif
_init:
	push {r0,lr}

.section .fini
.global _fini
.type _fini,%function
.ifndef __LITEOS__
.balign 4
.endif
_fini:
	push {r0,lr}

.include "crtbrand.s"

.syntax unified

.section .init
.global _init
.type _init,%function
.balign 4
_init:
	push {r0,lr}

.section .fini
.global _fini
.type _fini,%function
.balign 4
_fini:
	push {r0,lr}

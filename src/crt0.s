.arm
.align 4
.global _start

.section .text.start, "x"

_start:
	CPSID aif
	ldr sp, =_stack

	adr r3, main_offset
	ldr r2, [r3]
	add r2, r3
	blx r2

main_offset:
.word main-.


	.text
	.align 3
Lstack_chk_guard:
	.8byte 0x1234567890abcdef

	.section .data.rel.ro
	.align 3
	.global L1acb60
	.global L1ac730
L1acb60:
	.8byte Lstack_chk_guard
L1ac730:
	.8byte o_unimplemented

	.global tp_start
	.global tp_end
tp_start:
	orr	x3, x3, x3
	ret
tp_end:
	orr x4, x4, x4
	ret

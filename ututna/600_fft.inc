	.text
	.align 3

LC_14a8e8:
	.8byte 0xC00921FB54442D18
LC_14a8d0:
	.8byte 0xC01921FB54442D18
LC_14a8d8:
	.4byte L56e50 - LC_14a8d8
	.4byte L56fe4 - LC_14a8d8
	.4byte L5711c - LC_14a8d8
	.4byte L57290 - LC_14a8d8
LC_1ac730:
	.8byte 0
LC_stack_chk_guard:
	.8byte 0x1234567890abcdef

LC_14a8f0:
LC_14a915:
	.string "error"

	.data.rel.ro
	.align 3
LC_1acb60:
	.8byte LC_stack_chk_guard

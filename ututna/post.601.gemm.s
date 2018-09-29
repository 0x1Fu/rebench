_Z21test_functional_sgemmjPfS_:
_Z21test_functional_dgemmjPdS_:
	mov x0, #0
	ret

	.align 4
LC_stack_chk_guard:
	.8byte 0x1234567890abcdef
LC_1acb60:
	.8byte LC_stack_chk_guard
LC_14a8b0:
	.8byte 0x100000000
	.8byte 0x300000002
LC_14a8c0:
	.8byte 0x500000004
	.8byte 0x700000006
LC_148bf0:
	.8byte 0x412E848000000000
LC_14a940:
	.8byte 0x4180000000000000
LC_14a948:
	.8byte 0x41CDCD6500000000
LC_14a950:
	.8byte 0x100000000
LC_14a958:
	.8byte 0x300000002

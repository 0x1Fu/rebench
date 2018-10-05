	.text
	.align 3
_Z21test_functional_sgemmjPfS_:
_Z21test_functional_dgemmjPdS_:
	mov x0, #0
	ret

	.align 4
L14a8b0:
	.8byte 0x100000000
	.8byte 0x300000002
L14a8c0:
	.8byte 0x500000004
	.8byte 0x700000006
L148bf0:
	.8byte 0x412E848000000000
L14a940:
	.8byte 0x4180000000000000
L14a948:
	.8byte 0x41CDCD6500000000
L14a950:
	.8byte 0x100000000
L14a958:
	.8byte 0x300000002
Lstack_chk_guard:
	.8byte 0x1234567890abcdef

	.data.rel.ro
	.align 3
L1acb60:
	.8byte Lstack_chk_guard

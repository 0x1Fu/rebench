	.text
	.align 3

L148bf0:
	.8byte 0x412E848000000000

L159170:
	.include "L159170.s"

	.align 4
L159b00:
	.8byte 0x707050503020202
	.8byte 0xB0B0B0B

	.align 1
L15b330:
	.2byte 0xC0
	.2byte 0x166
	.2byte 0
	.2byte 0x166
	.2byte 0x15F
	.2byte 0x166
	.2byte 0
	.2byte 0x15B
	.2byte 0x15B
	.2byte 0x15F
	.2byte 0
	.2byte 0xC2
	.2byte 0xC2
	.2byte 0xC2
	.2byte 0xB8
	.2byte 0

	.align 1
L15b350:
	.2byte 2
	.2byte 2
	.2byte 0x6C
	.2byte 0
	.2byte 0
	.2byte 0x10D
	.2byte 0x6C
	.2byte 0
	.2byte 0x10D
	.2byte 0
	.2byte 0x6C
	.2byte 0x70
	.2byte 0x70
	.2byte 0x70
	.2byte 0x64
	.2byte 0

	.align 3
L4b790:
L1283f0:
	bl o_unimplemented

	.section .data.rel.ro
	.align 3

// 1a6de8
vtable_CMapTest:
	.8byte 0
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte 0
	.8byte 0

// 1a6e28
vtable_CMapTestImp:
	.8byte 0
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte _ZN12CMapTestImplISt3mapIj12SDataElementSt4lessIjESaISt4pairIKjS1_EEESt17_Rb_tree_iteratorIS6_EE4testEPj
	.8byte _ZN8CMapTest9fini_testEv
	.8byte _ZNK8CMapTest16get_ops_per_testEv
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte 0

vtable_1a6e98:
	.8byte 0
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte _ZN12CMapTestImplISt13unordered_mapIj12SDataElementSt4hashIjESt8equal_toIjESaISt4pairIKjS1_EEENSt8__detail14_Node_iteratorIS8_Lb0ELb0EEEE4testEPj
	.8byte _ZN8CMapTest9fini_testEv
	.8byte _ZNK8CMapTest16get_ops_per_testEv
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte 0

L1ac430:
	.8byte dummy
L1ac8e8:
	.8byte vtable_1a6e98
L1acea8:
	.8byte vtable_CMapTest
L1acee8:
	.8byte vtable_CMapTestImp

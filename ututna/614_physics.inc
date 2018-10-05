	.text
	.align 3
LC_148bf0:
	.8byte 0x412E848000000000
LC_149180:
	.8byte 0x40C3880000000000
LC_14f5a0:
	.include "LC_14f5a0.s"
LC_14a898:
	.8byte 0x41DFFFFFFFC00000
LC_14a948:
	.8byte 0x41CDCD6500000000
LC_14b0c8:
	.8byte 0x3FEE666666666666
LC_14f570:
	.8byte 0x3FA999999999999A
LC_14f578:
	.8byte 0x7FF0000000000000
LC_14f580:
	.8byte 0xC072C00000000000
LC_14f588:
	.8byte 0x4079000000000000
//LC_14f590:
	//.8byte 0x3F81111111111111
LC_14f598:
	.8byte 0x4082C00000000000
LC_14f8e8:
	.8byte 0x400921FB54442D18
LC_stack_chk_guard:
	.8byte 0x1234567890abcdef

	.align 4
LC_14fa70:
	.8byte 0x3FF0000000000000
	.8byte 0
LC_14fa80:
	.8byte 0x8000000000000000
	.8byte 0x3FF0000000000000
LC_14fa90:
	.8byte 0
	.8byte 0x7FF0000000000000
LC_14faa0:
	.8byte 0x7FF0000000000000
	.8byte 0
LC_150820:
	.8byte 0x3FB99999A0000000
	.8byte 0x3F5D71320CDF4363

LC_14fab0:
LC_14fabf:
LC_14fb18:
LC_14fb9a:
LC_14fbc6:
LC_14fbfd:
LC_14fc1d:
LC_14fc3f:
LC_14fc4e:
LC_1500b9:
LC_1500c3:
LC_15011b:
LC_150526:
LC_1505f2:
LC_150605:
LC_15062f:
LC_150640:
LC_150830:
LC_15083f:
LC_150899:
LC_1508ec:
LC_150911:
LC_150972:
LC_150988:
LC_1509dc:
LC_1509ea:
LC_150a3d:
LC_150a52:
LC_150aa5:
LC_150ab2:
LC_150c21:
LC_150c44:
LC_150c92:
LC_150cb6:
LC_150cee:
LC_150d0f:
LC_1512a0:
LC_1512fe:
LC_151323:
LC_15134c:
LC_151367:
LC_1513c8:
	.string "error"

	.align 3
L8b5c0:
L8d318:
L8ddfc:
L9be24:
L9be74:
L9c194:
L9d150:
L9d828:
L9d8f8:
La0770:
La13f4:
cpArbiterIgnore:
cpBodyAccumulateMassFromShapes:
cpBodyActivateStatic:
cpBodyKineticEnergy:
cpMessage:
cpSpaceActivateBody:
	bl o_unimplemented

	.data.rel.ro
	.align 3
LC_1ac3e0:
	.8byte cpSpaceCollideShapes
LC_1ac520:
	.8byte cpBodyUpdateVelocity
LC_1ac588:
	.8byte cpShapeGetBB
LC_1ac608:
	.8byte o_unimplemented
LC_1ac670:
	.8byte free
LC_1aca40:
	.8byte LC_1ad0e0
LC_1acd28:
	.8byte cpBodyUpdatePosition
LC_1acf50:
	.8byte cpSpaceArbiterSetFilter
LC_1acb60:
	.8byte LC_stack_chk_guard
LC_1acfc8:
	.8byte cpShapeUpdateFunc

LC_1a7308:
	.8byte 0
	.8byte L9a820
	.8byte 0
	.8byte o_unimplemented
	.8byte o_unimplemented
LC_1a7198:
	.8byte L90058
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
LC_1ad088:
	.8byte L8d814
	.8byte o_unimplemented
	.8byte L8d858
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte o_unimplemented
LC_1ad0e0:
	.8byte 0xFFFFFFFFFFFFFFFF
	.8byte 0xFFFFFFFFFFFFFFFF
	.8byte L9bc4c
	.8byte L9bc4c
	.8byte L9bc54
	.8byte L9bc54
	.8byte 0
LC_1ad118:
	.8byte L9ebe0
	.8byte L9eca4
	.8byte L9ecac
	.8byte o_unimplemented
	.8byte L9ed30
	.8byte L9ed8c
	.8byte o_unimplemented
	.8byte o_unimplemented
	.8byte L9ef2c
	.8byte o_unimplemented
	.8byte o_unimplemented

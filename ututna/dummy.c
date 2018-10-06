#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

int o_setjmp(jmp_buf env) {
	return 0;
}

#define UNIMPLEMENTED(func) \
void func() { \
	fprintf(stderr, "unimplemented: %s\n", __func__); \
	exit(-1); \
}

UNIMPLEMENTED(o_unimplemented)
UNIMPLEMENTED(o_fwrite)
UNIMPLEMENTED(o_exit)
UNIMPLEMENTED(o_abort)

UNIMPLEMENTED(o___stack_chk_fail)
UNIMPLEMENTED(o___sfp_handle_exceptions)
UNIMPLEMENTED(o___cxa_begin_catch)
UNIMPLEMENTED(o___cxa_end_catch)
UNIMPLEMENTED(o___cxa_rethrow)
UNIMPLEMENTED(o__Unwind_Resume)

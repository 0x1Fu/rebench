.macro stub sym

	.global \sym
\sym:
	.8byte 0
.endm

	.section .data.rel.ro
	.align 3

stub main

stub L1ac730
stub L1acb60
stub L14f590
stub o_abort
stub o___cxa_begin_catch
stub o___cxa_end_catch
stub o___cxa_rethrow
stub o_exit
stub o_fwrite
stub o_setjmp
stub o___sfp_handle_exceptions
stub o___stack_chk_fail
stub o_unimplemented
stub o__Unwind_Resume
stub s_calloc
stub s_cos
stub s_crc32
stub s_fprintf
stub s_fputc
stub s_free
stub s_gettimeofday_begin
stub s_gettimeofday_end
stub s_inflate
stub s_inflateEnd
stub s_inflateInit_
stub s_inflateReset
stub s_malloc
stub s_memcmp
stub s_memcpy
stub s_memset
stub s_pow
stub srand
stub s_realloc
stub s_sin
stub s_sprintf
stub s_sqrt
stub s_StartStopwatch
stub s_StopStopwatch
stub s__ZdaPv
stub s__ZdlPv
stub s__Znam
stub s__Znwm
stub _Z15test_const_timeR8CMapTestd

stub clock_gettime
stub dummy
stub gemm_free
stub gemm_malloc
stub gettimeofday
stub rand
stub time

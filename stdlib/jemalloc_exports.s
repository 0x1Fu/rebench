.macro export sym ptr

	.global \sym
\sym:
	adrp    x16, \ptr
	ldr     x17, [x16,#:lo12:\ptr]
	br      x17
.endm

	.text
	.align 3

export s_malloc   je_malloc_ptr
export s_calloc   je_calloc_ptr
export s_realloc  je_realloc_ptr
export s_free     je_free_ptr

export s__Znam    je_malloc_ptr
export s__Znwm    je_malloc_ptr

export s__ZdaPv   je_free_ptr
export s__ZdlPv   je_free_ptr

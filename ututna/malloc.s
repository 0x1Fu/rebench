.macro bootstrap sym bootstrap_sym

	.global \sym
\sym:
	b \bootstrap_sym
.endm

	.text
	.align 3

bootstrap malloc   bootstrap_malloc
bootstrap calloc   bootstrap_calloc
bootstrap realloc  bootstrap_realloc
bootstrap free     bootstrap_free

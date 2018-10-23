.macro export sym ptr

	.global \sym
\sym:
	adrp    x16, \ptr
	ldr     x17, [x16,#:lo12:\ptr]
	br      x17
.endm

	.text
	.align 3
export s_memset   memset_ptr
export s_memcpy   memcpy_ptr
export s_memcmp   memcmp_ptr
export s_memmove  memmove_ptr

export s_strncmp  strncmp_ptr
export s_strcmp   strcmp_ptr
export s_strchr   strchr_ptr
export s_strlen   strlen_ptr
export s_strtol   strtol_ptr

export s_sin      sin_ptr
export s_cos      cos_ptr
export s_pow      pow_ptr
export s_sqrt     sqrt_ptr

export s_crc32         crc32_ptr
export s_inflate       inflate_ptr
export s_inflateEnd    inflateEnd_ptr
export s_inflateInit_  inflateInit__ptr
export s_inflateReset  inflateReset_ptr

export s_sprintf  sprintf_ptr
export s_fprintf  fprintf_ptr
export s_fputc    fputc_ptr

#define UNIMPLEMENTED(func) \
	void func() { }

#define DUMMY(func) \
	extern int func(int);

DUMMY(memset)
DUMMY(memcpy)
DUMMY(memcmp)
DUMMY(memmove)
DUMMY(strncmp)
DUMMY(strcmp)
DUMMY(strchr)
DUMMY(strlen)
DUMMY(strtol)
DUMMY(sin)
DUMMY(cos)
DUMMY(pow)
DUMMY(sqrt)
DUMMY(sprintf)
DUMMY(fprintf)
DUMMY(fputc)
DUMMY(crc32)
DUMMY(inflate)
DUMMY(inflateEnd)
DUMMY(inflateInit_)
DUMMY(inflateReset)

UNIMPLEMENTED(__memset_chk_fail)
UNIMPLEMENTED(async_safe_fatal_va_list)

int main(int argc, char *argv[]) {
	int ret = argc;

	ret += memset(ret);
	ret += memcpy(ret);
	ret += memcmp(ret);
	ret += memmove(ret);
	ret += strncmp(ret);
	ret += strcmp(ret);
	ret += strchr(ret);
	ret += strlen(ret);
	ret += strtol(ret);
	ret += sin(ret);
	ret += cos(ret);
	ret += pow(ret);
	ret += sqrt(ret);
	ret += sprintf(ret);
	ret += fprintf(ret);
	ret += fputc(ret);
	ret += crc32(ret);
	ret += inflate(ret);
	ret += inflateEnd(ret);
	ret += inflateInit_(ret);
	ret += inflateReset(ret);

	return ret;
}

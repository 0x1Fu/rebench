#define UNIMPLEMENTED(func) \
	void func() { }

#define DUMMY(func) \
	extern int func(int);

DUMMY(je_malloc)
DUMMY(je_calloc)
DUMMY(je_realloc)
DUMMY(je_free)

UNIMPLEMENTED(atexit)
UNIMPLEMENTED(async_safe_fatal_no_abort)

int main(int argc, char *argv[]) {
	int ret = argc;

	ret += je_malloc(ret);
	ret += je_calloc(ret);
	ret += je_realloc(ret);
	ret += je_free(ret);

	return ret;
}

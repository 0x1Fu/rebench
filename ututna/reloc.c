#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

#ifndef DEBUG_RELOC
#define DEBUG_RELOC 0
#endif

#if DEBUG_RELOC
#define debug(format, ...)  fprintf(stderr, format, ##__VA_ARGS__)
#else
#define debug(...)
#endif

#define ALIGN(size, align)    ((size) & (~(align - 1)))
#define ALIGN_UP(size, align) ((size + align - 1) & (~(align - 1)))

#define PAGE_SIZE  4096

struct entry {
    uint64_t offset;
    uint64_t addr;
    uint64_t type;
};

static void do_reloc(const struct entry *relocs, uint64_t start)
{
	const struct entry *e = relocs;
	while (e->offset) {
		uint64_t offset = e->type == R_AARCH64_RELATIVE ? start : 0;

		debug("reloc: %lx %lx\n", e->offset + start, e->addr + offset);
		*(uint64_t *)(e->offset + start) = e->addr + offset;

		e++;
	}
}

static void dump_maps()
{
	char buf[128];
	sprintf(buf, "cat /proc/%d/maps", getpid());
	system(buf);
}

void reloc(uint64_t bin_start, uint64_t bin_size,
				uint64_t addr, uint64_t size,
				uint64_t rom_start,
				uint64_t text_start, uint64_t text_size,
				uint64_t bss_start, uint64_t bss_size,
				const struct entry *relocs)
{
	debug("bin_start = 0x%lx\n", bin_start);
	debug("bin_size  = 0x%lx\n", bin_size);

	char *ptr = mmap((void *)addr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (ptr != (void *)addr) {
		fprintf(stderr, "[stdlib] error: mmap, returns %p, expected %lx\n", ptr, addr);
		dump_maps();
		exit(-1);
	}

	memcpy(ptr + rom_start, (void *)bin_start, bin_size);

	memset(ptr + bss_start, 0, bss_size);
	debug("bss_start  = %p\n", ptr + bss_start);
	debug("bss_end    = %p\n", ptr + bss_start + bss_size);

	char *reloc_text_start = (char *)ALIGN(addr + text_start, PAGE_SIZE);
	char *reloc_text_end = (char *)ALIGN_UP(addr + text_start + text_size, PAGE_SIZE);
	debug("text_start = 0x%lx 0x%p\n", text_start, reloc_text_start);
	debug("text_end   = 0x%lx 0x%p\n", text_start + text_size, reloc_text_end);

	mprotect(reloc_text_start, reloc_text_end - reloc_text_start, PROT_READ | PROT_EXEC);

	do_reloc(relocs, addr);
}

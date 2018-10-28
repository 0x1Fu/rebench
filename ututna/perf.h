#ifndef __perf_h__
#define __perf_h__

#ifndef SEMIHOSTING
void perf_config(char type, int event);
void perf_init();
void perf_begin();
void perf_end();
void perf_print_headers();
void perf_print();
#endif

#endif

#ifndef C_UTIL_H
#define C_UTIL_H

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#include <stdio.h>

// Debug print helper
#define DEBUG_PRINT(fmt, ...) do {						\
	if (DEBUG_TEST) fprintf(stderr, "DEBUG: %s:%d %s(): " fmt, __FILE__,	\
		__LINE__, __FUNCTION__, __VA_ARGS__); } while (0)

#define MAXSTRLEN 128

int sstrcpy(char *dst, const char *src, int len);
char *dynstr(const char *);

#endif //C_UTIL_H

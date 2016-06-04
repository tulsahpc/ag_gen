#ifndef C_UTIL_H
#define C_UTIL_H

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

// Debug print helper
#define DEBUG_PRINT(fmt, ...) do {						\
	if (DEBUG_TEST) fprintf(stderr, "DEBUG: %s:%d %s(): " fmt, __FILE__,	\
		__LINE__, __FUNCTION__, __VA_ARGS__); } while (0)

#endif //C_UTIL_H

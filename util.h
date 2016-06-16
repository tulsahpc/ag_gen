/** \file util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

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

/* Added because this is a non-standard C function.
   Windows equivalent is strtok_s.
*/
extern char *strtok_r(char *, const char *, char **);

int sstrcpy(char *dst, const char *src, int len);
char *dynstr(const char *);

#endif //C_UTIL_H

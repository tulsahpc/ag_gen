/** \file util.h
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#ifndef UTIL_COMMON_H
#define UTIL_COMMON_H

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#include <stdio.h>

#include "asset.h"

#define DEBUG_PRINT(fmt, ...) do {						\
	if (DEBUG_TEST) fprintf(stderr, "DEBUG: %s:%d %s(): " fmt, __FILE__,	\
		__LINE__, __FUNCTION__, __VA_ARGS__); } while (0)

#define TEST_PRINT(name, status) do {						\
	fprintf(stderr, "%s: %s\n", name, status?"Failure":"Success");		\
		} while (0)

#define MAXSTRLEN 128

char alphabet[35] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'o',
	'p', 'q', 'r', 's',
	't', 'u', 'v', 'w',
	'x', 'u', 'z'
};

/* Added because this is a non-standard C function.
   Windows equivalent is strtok_s.
*/
extern char *strtok_r(char *, const char *, char **);

int sstrcpy(char *dst, const char *src, int len);
char *dynstr(const char *);

#endif //UTIL_COMMON_H

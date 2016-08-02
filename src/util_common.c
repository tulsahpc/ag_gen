/** \file util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <string.h>

#include "util_common.h"

// assertions
// dst.size >= src.size
// len <= src.size
int sstrcpy(char *dst, const char *src, int len)
{
	if(dst == NULL || src == NULL)
		return -1;

	char counter = 0;

	char c;
	while((c = *(src+counter)) != '\0' && counter < len)
		*(dst+counter++) = c;

	*(dst+counter) = '\0';
	return 0;
}

// Always free the string when no longer required
char *dynstr(const char *str)
{
	int str_len = strlen(str);
	char *new_str = malloc(sizeof(char)*(str_len+1));
	if(new_str == NULL)
		return (char *) -1;

	sstrcpy(new_str, str, str_len);
	return new_str;
}

/** \file util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <string.h>

#include "util_common.h"

unsigned int base_convert_string(char *data, int base)
{
	int len = strlen(data);
	int total = 0;
	for(int i=0; i<len; i++) {
		unsigned int next_num = 0;
		if(data[i] <= 'z' && data[i] >= 'a') {
			next_num = data[i] - 87;
		} else if(data[i] <= 'Z' && data[i] >= 'A') {
			next_num = data[i] - 55;
		} else if(data[i] <= '9' && data[i] >= '0') {
			next_num = data[i] - '0';
		} else {
			printf("Malformed Input\n");
			exit(1);
		}
		total = total * base + next_num;
	}
	return total;
}

char *base_convert_int(int num, int base)
{
	struct List st;
	while(num > 0) {
		list_push(&st, (void *)(unsigned long)(num % base));
		num = num / base;
	}

	int size = list_size(&st);
	char *converted = malloc(size * sizeof(char) + 1);
	for(int i=0; i<size; i++) {
		converted[i] = alphabet[(int)list_pop(&st)];
	}
	converted[size] = '\0';

	return converted;
}

char *base_convert(char *num, int from, int to)
{
	return base_convert_int(base_convert_string(num, from), to);
}

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

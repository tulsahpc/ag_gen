#include <stdlib.h>
#include <string.h>
#include "util.h"

int sstrcpy(char *dst, const char *src, int len)
{
	char counter = 0;
	char c;

	if(dst == NULL || src == NULL)
		return 1;

	while((c = *(src+counter)) != '\0' && counter < len)
		*(dst+counter++) = c;

	*(dst+counter) = '\0';
	return 0;
}

char *dynstr(const char *str)
{
	if(str == NULL)
		return NULL;

	int strLen = strlen(str);
	char *newStr = malloc(sizeof(char)*(strLen+1));
	if(newStr == NULL)
		return NULL;

	sstrcpy(newStr, str, strLen);
	return newStr;
}

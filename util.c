/** \file util.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include <stdlib.h>
#include <string.h>

#include "ag_asset.h"
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

struct AGBindingList *permute_r(struct AGAssetList *assets, int num_params)
{
	int num_assets = assets->len;
	int pmtns = num_assets * num_params;

	struct AGBindingList *bindings = malloc(sizeof(struct AGBindingList));
	bindings->size = pmtns;
	bindings->list = malloc(pmtns * sizeof(int[num_params]));



	return bindings;
}

void AGBindingsFree(struct AGBinding *bindings)
{
	free(bindings);
}

void AGBindingsPrint(struct AGBinding *bindings)
{
	int len = bindings->size;
	for(int i=0; i<len; i++) {

	}
}

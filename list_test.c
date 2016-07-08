/** \file list_test.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include "util_list.h"

int main()
{
	struct List *list = ListNew();

	ListAppend(list, "Hello");
	ListAppend(list, "I");
	ListAppend(list, "am");
	ListAppend(list, "a");
	ListAppend(list, "string.");

	ListPrint(list, char*, "%s ");
	ListFree(list);

	struct List *new_list = ListNew();

	ListAppend(new_list, (void *)1);
	ListAppend(new_list, (void *)2);
	ListAppend(new_list, (void *)3);
	ListAppend(new_list, (void *)4);
	ListAppend(new_list, (void *)5);

	ListPrint(new_list, int, "%d ");

	struct ListArray *arr = ListAsArray(new_list);

	for(int i=0; i<arr->size; i++)
		printf("%d\n", (int)arr->val[i]);

	ListArrayFree(arr);
	ListFree(new_list);

	return 0;
}

/** \file list_test.c
 * \author Kyle Cook <kylecook80@gmail.com>
 * \date June 2016
 * \copyright Copyright (C) The University of Tulsa - All Rights Reserved. Unauthorized copying or distribution of this file is strictly prohibited.
 */

#include "util_list.h"

int main()
{
	struct List *list = list_new();

	list_push(list, "Hello");
	list_push(list, "I");
	list_push(list, "am");
	list_push(list, "a");
	list_push(list, "string.");

	list_print(list, char*, "%s ");
	list_free(list);

	struct List *new_list = list_new();

	list_push(new_list, (void *)1);
	list_push(new_list, (void *)2);
	list_push(new_list, (void *)3);
	list_push(new_list, (void *)4);
	list_push(new_list, (void *)5);

	list_print(new_list, int, "%d ");

	struct ListArray *arr = list_to_array(new_list);

	for(int i=0; i<arr->size; i++)
		printf("%d\n", (int)arr->val[i]);

	listarray_free(arr);
	list_free(new_list);

	return 0;
}

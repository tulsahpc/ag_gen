#include <stdlib.h>
#include <stdio.h>
#include "util_hash.h"

int main()
{
	struct HashTable *table = HashTableNew();

	HashTableAdd(table, "hello", (void *)12345);
	int value = (int)HashTableFind(table, "hello");
	printf("%d\n", value);

	HashTableAdd(table, "goodbye", "Thing");
	char *str = HashTableFind(table, "goodbye");
	printf("%s\n", str);

	HashTableFree(table);

	return 0;
}

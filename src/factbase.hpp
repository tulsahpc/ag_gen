#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>
#include "fact.hpp"

struct KeyVal {
	int key;
	int val;
};

class Factbase {
	std::vector<KeyVal> db;
public:
	Factbase(void);

	int check(int key);
	void add(int key, int value);
	int get(int key);
	int remove(int key);
};

#endif

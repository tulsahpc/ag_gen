#ifndef FACTBASE_HPP
#define FACTBASE_HPP

#include <vector>
#include "fact.hpp"

class Factbase {
	std::vector<int> factlist;
public:
	Factbase(void);

	int check(int key);
	void add(int key, int value);
	int get(int key);
	int remove(int key);
};

#endif

#ifndef UTIL_ODOMETER_H
#define UTIL_ODOMETER_H

#include <vector>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

class Odometer {
	int n;
	int k;
	std::vector<int*> perms;
	int idx_state = 0;
public:
	Odometer(int n, int k);
	~Odometer();

	void calc(void);
	int num_perms(void);
	int perm_length(void);
	void print(void);
	int* next(void);
};

#ifdef __cplusplus
}
#endif

#endif //UTIL_ODOMETER_H

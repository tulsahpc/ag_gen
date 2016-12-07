#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

#include "util_common.hpp"
#include "util_odometer.hpp"

using namespace std;

Odometer::Odometer(int n_in, int k_in) : n(n_in), k(k_in) {}

Odometer::~Odometer()
{
	for(auto perm : perms) {
		delete [] perm;
	}
}

void Odometer::calc()
{
	int curr_idx = 0;
	int perm_idx = 0;

	int *orig_perm = new int[n];
	perms.push_back(orig_perm);
	int *last_perm = orig_perm;

	for(int i=0; i<n; i++) {
		orig_perm[i] = 0;
	}

	while(perm_idx < num_perms()) {
		int* curr_perm = new int[n];
		for(int i=0; i<n; i++) {
			curr_perm[i] = last_perm[i];
		}

		perms.push_back(curr_perm);

		while(curr_idx < n && !(curr_perm[curr_idx] < k-1)) {
			curr_idx++;
		}

		perm_idx++;

		if(curr_idx < n) {
			curr_perm[curr_idx]++;
			for(int i=0; i<curr_idx; i++) {
				curr_perm[i] = 0;
			}
			curr_idx = 0;
			last_perm = curr_perm;
		} else {
			break;
		}
	}
}

void Odometer::print(void)
{
	for(auto perm : perms) {
		for(int i=n-1; i>=0; i--) {
			cout << perm[i] << " ";
		}
		cout << endl;
	}
}

inline int Odometer::num_perms(void)
{
	return pow(k, n);
}

int *Odometer::next(void)
{
	if(idx_state < num_perms()) {
		return perms[idx_state++];
	}
	return nullptr;
}

int Odometer::perm_length(void)
{
	return n;
}

#ifndef UTIL_ODOMETER_HPP
#define UTIL_ODOMETER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <cmath>
#include <memory>

// N is the width of the permutation
// K is the number of elements
template <int n,int k>
class Odometer {
	std::vector<std::array<int, n> > perms;
	int idx_state = 0;

public:
	Odometer() {
		int curr_idx = 0;
		int perm_idx = 0;

		std::array<int, n> orig_perm;
		std::array<int, n>& last_perm = orig_perm;

		for(int i=0; i<n; i++) {
			orig_perm[i] = 0;
		}

		while(perm_idx < length()) {
			std::array<int, n> curr_perm;
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

	void print(void) {
		for(std::array<int,n> perm : perms) {
			for(int num : perm) {
				std::cout << num;
			}
			std::cout << std::endl;
		}
	}

	int length(void) {
		return pow(n,k);
	}

	std::array<int, n> next(void) {
		return perms[idx_state++];
	}

	int perm_length(void) {
		return n;
	}

	std::vector<std::array<int, n> > get_all() {
		return perms;
	}
};

#endif //UTIL_ODOMETER_HPP

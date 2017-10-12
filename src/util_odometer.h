#ifndef UTIL_ODOMETER_HPP
#define UTIL_ODOMETER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>

// N is the width of the permutation
// K is the number of elements
class Odometer {
    std::vector<std::vector<int> > perms;
    int idx_state = 0;
    int n, k;

public:
    Odometer(int in_n, int in_k) {
        n = in_n;
        k = in_k;

        int curr_idx = 0;
        int perm_idx = 0;

        std::vector<int> orig_perm(n);
        std::vector<int> &last_perm = orig_perm;

        for (int i = 0; i < n; i++) {
            orig_perm[i] = 0;
        }

        while (perm_idx < length()) {
            std::vector<int> curr_perm(n);
            for (int i = 0; i < n; i++) {
                curr_perm[i] = last_perm[i];
            }

            perms.push_back(curr_perm);

            while (curr_idx < n && !(curr_perm[curr_idx] < k - 1)) {
                curr_idx++;
            }

            perm_idx++;

            if (curr_idx < n) {
                curr_perm[curr_idx]++;
                for (int i = 0; i < curr_idx; i++) {
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
        for (std::vector<int> perm : perms) {
            for (int num : perm) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }

    const int length(void) {
        return pow(k, n);
    }

    std::vector<int> next(void) {
        return perms[idx_state++];
    }

    void reset(void) {
        idx_state = 0;
    }

    int perm_length(void) {
        return n;
    }

    std::vector<std::vector<int> > get_all() {
        return perms;
    }
};

#endif //UTIL_ODOMETER_HPP

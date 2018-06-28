#ifndef UTIL_ODOMETER_HPP
#define UTIL_ODOMETER_HPP

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// N is the width of the permutation
// K is the number of elements
class Odometer {
    std::vector<std::vector<size_t>> perms;
    size_t idx_state = 0;
    size_t n, k;

  public:
    Odometer(size_t in_n, size_t in_k) {
        n = in_n;
        k = in_k;

        size_t curr_idx = 0;
        size_t perm_idx = 0;

        std::vector<size_t> orig_perm(n);
        std::vector<size_t> &last_perm = orig_perm;

        for (size_t i = 0; i < n; i++) {
            orig_perm[i] = 0;
        }

        while (perm_idx < length()) {
            std::vector<size_t> curr_perm(n);
            for (size_t i = 0; i < n; i++) {
                curr_perm[i] = last_perm[i];
            }

            perms.push_back(curr_perm);

            while (curr_idx < n && curr_perm[curr_idx] >= k - 1) {
                curr_idx++;
            }

            perm_idx++;

            if (curr_idx < n) {
                curr_perm[curr_idx]++;
                for (size_t i = 0; i < curr_idx; i++) {
                    curr_perm[i] = 0;
                }
                curr_idx = 0;
                last_perm = curr_perm;
            } else {
                break;
            }
        }
    }

    void print() {
        for (std::vector<size_t> perm : perms) {
            for (auto num : perm) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }

    unsigned long length() { return pow(k, n); }

    std::vector<size_t> next() { return perms[idx_state++]; }

    const std::vector<size_t> &operator[](int idx) const { return perms[idx]; }

    void reset() { idx_state = 0; }

    size_t perm_length() { return n; }

    std::vector<std::vector<size_t>> get_all() { return perms; }

    size_t get_size() { return perms.size(); }

    std::vector<size_t> const &GetAt(size_t i) const {

        if (i < perms.size())
            return perms[i];
        throw std::out_of_range("index out of range");
    }
};

class Odometer_iterator {
    Odometer od;
    size_t index;

  public:
    Odometer_iterator(Odometer _od, size_t const i) : od(_od), index(i) {}

    bool operator!=(Odometer_iterator const &other) const {

        return index != other.index;
    }

    std::vector<size_t> const &operator*() const { return od.GetAt(index); }

    Odometer_iterator const &operator++() {

        ++index;
        return *this;
    }
};

inline Odometer_iterator begin(Odometer od) { return Odometer_iterator(od, 0); }

inline Odometer_iterator end(Odometer od) {

    return Odometer_iterator(od, od.get_size());
}

#endif // UTIL_ODOMETER_HPP

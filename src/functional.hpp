#ifndef AG_GEN_FUNCTIONAL_HPP
#define AG_GEN_FUNCTIONAL_HPP

#include <vector>

std::vector<int> zip(std::vector<int> v1, std::vector<int> v2) {
    if(v1.size() != v2.size()) {
        cerr << "vectors must be same size." << endl;
        exit(1);
    }

    std::vector<int> zipped_vector(v1.size());

}

#endif //AG_GEN_FUNCTIONAL_HPP

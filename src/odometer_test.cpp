#include <set>
#include "util/odometer.h"

int main() {
    Odometer<std::set<int>> od(3, 3);
    for(auto perm : od) {
        for(auto i : perm) {
            std::cout << i.;
        }
        std::cout << "\n";
    }
    return 0;
}

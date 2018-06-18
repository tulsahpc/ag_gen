// NetGen is a tool used to randomly generate network models
// given certain constraints. More to come...

#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int sumMapValues(int val, std::pair<const int, int> p) {
    return val + p.second;
}

int main(int argc, char **argv) {
    int numAssets = 100;
    int numFacts = 1000;

    auto numQuals = static_cast<int>(trunc(numFacts * 0.7));
    auto numTopos = static_cast<int>(numFacts - numQuals);

    std::random_device rd;
    std::mt19937 gen(rd());

    // Mean of 20 facts, stddev of 7
    std::uniform_int_distribution<> d(8, 12);

    std::vector<int> assetFacts{numAssets};
    for (int i = 0; i < numAssets; i++) {
        assetFacts[i] = static_cast<int>(std::round(d(gen)));
        std::cout << assetFacts[i] << std::endl;
    }

    int sum = std::accumulate(assetFacts.begin(), assetFacts.end(), 0);
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}

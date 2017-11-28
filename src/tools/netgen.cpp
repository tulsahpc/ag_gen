// NetGen is a tool used to randomly generate network models
// given certain constraints. More to come...

#include <iostream>
#include <map>
#include <random>
#include <numeric>
#include <cmath>

int sumMapValues(int val, std::pair<const int, int> p) {
    return val + p.second;
}

int main(int argc, char** argv) {
    int numAssets = 100;
    int numFacts = 1000;
    int numQuals = numFacts * 0.7;
    int numTopos = numFacts - numQuals;

    std::random_device rd;
    std::mt19937 gen(rd());

    // Mean of 20 facts, stddev of 7
    std::uniform_int_distribution<> d(12,7);

    std::map<int, int> assetFacts;
    for(int i=0; i<numAssets; i++) {
        assetFacts[i] = std::round(d(gen));
        std::cout << assetFacts[i] << std::endl;
    }

    int sum = std::accumulate(assetFacts.begin(), assetFacts.end(), 0, sumMapValues);
    std::cout << sum << std::endl;

    return 0;
}

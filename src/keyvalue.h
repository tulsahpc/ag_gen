#ifndef KEYVALUE_HPP
#define KEYVALUE_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

class Keyvalue {
    int length {0};
    std::unordered_map<std::string, int> hash_table;
    std::vector<std::string> str_vector;
    
    template<typename U, typename = std::string>
    struct can_get_name : std::false_type {};
    
    template<typename U>
    struct can_get_name<U, decltype(std::declval<U>().get_name())> : std::true_type {};

public:

    template<typename U>
    typename std::enable_if<can_get_name<U>::value, void>::type
    insert(U &target) {
        hash_table[target.get_name()] = length;
        str_vector.push_back(target.get_name());
        length++;
    }

    template<typename U>
    typename std::enable_if<!can_get_name<U>::value, void>::type
    insert(U &target) {
        hash_table[target] = length;
        str_vector.push_back(target);
        length++;
    }

    int operator[](const std::string &str) {
        return hash_table[str];
    }

    std::string operator[](int num) {
        return str_vector.at(num);
    }

    int size() const {
        return length;
    }
};

#endif //KEYVALUE_HPP

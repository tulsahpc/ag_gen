#ifndef KEYVALUE_HPP
#define KEYVALUE_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

template <typename T>
class Keyvalue {
	template <typename U, typename = std::string>
	struct can_get_name : std::false_type {};

	template <typename U>
	struct can_get_name<U, decltype(std::declval<U>().get_name())> : std::true_type {};

    int size;
    std::unordered_map<std::string, int> hash_table;
    std::vector<std::string> str_vector;

    void populate(std::vector<T>& list) {
		size = 0;
		for(T& item : list) {
			insert(item);
		}
    }

public:
	Keyvalue(std::vector<T>& preList) {
		populate(preList);
	}

	Keyvalue(std::vector<T>&& preList) {
		populate(preList);
	}

	template <typename U>
	typename std::enable_if<can_get_name<U>::value, void>::type
    insert(U& target) {
	    hash_table[target.get_name()] = size;
	    str_vector.push_back(target.get_name());
	    size++;
    }

    template <typename U>
	typename std::enable_if<!can_get_name<U>::value, void>::type
    insert(U& target) {
	    hash_table[target] = size;
	    str_vector.push_back(target);
	    size++;
    }

    int operator[](const std::string& str) {
    	return hash_table[str];
    }

    std::string operator[](u_long num) {
    	return str_vector.at(num);
    }

    int length() {
        return size;
    }
    
private:
    typedef std::vector<std::string> StringList;
    StringList strs;
public:
    typedef StringList::iterator iterator;
    typedef StringList::const_iterator const_iterator;
    
    iterator begin() {return strs.begin(); }
    iterator end() {return strs.end(); }
};

#endif //KEYVALUE_HPP

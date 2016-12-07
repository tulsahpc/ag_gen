//
// Created by archlord on 12/6/16.
//

#ifndef AG_GEN_HASHMAP_H
#define AG_GEN_HASHMAP_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "network.hpp"
#include "asset.hpp"
#include "exploit.hpp"
#include "util_db.h"

/*
 * At time of creation, this is not supposed to hold
 * any classes, as I will be using the std class
 * unsorted_map. This is just to help avoid conflicts in ag_gen
 */
template <class T>
class Keyvalue{
public:
    int size;
    std::unordered_map<std::string,int> hash_table; //Pass in string get int
    std::vector<std::string> str_vector; //pass in int get string
    Keyvalue<T>(std::vector<std::shared_ptr<T>> hash_targets);
    void insert( std::shared_ptr<T> target );
    //Get ID: pass string get int
    int get_id( std::string name);
    //Get Name: pass int get string
    std::string get_name( int id );
};

template <class T>
int Keyvalue<T>::get_id( std::string name ) {
    return hash_table[name];
}

template <class T>
std::string Keyvalue<T>::get_name( int id ) {
    return str_vector.at(id);
}

template <class T>
Keyvalue<T>::Keyvalue( std::vector<std::shared_ptr<T>> hash_targets ) {
    size = 0;
    for( auto item : hash_targets ){
        insert(item);
    }
}

template <class T>
void Keyvalue<T>::insert( std::shared_ptr<T> target ) {
    hash_table[target->name] = size;
    str_vector.push_back( target->name );
    size++;
    return;
}



#endif //AG_GEN_HASHMAP_H

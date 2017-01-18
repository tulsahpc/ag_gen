#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <memory>

template<class T>
using List = std::vector<std::unique_ptr<T> >;

#endif // TYPES_HPP

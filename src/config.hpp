#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <memory>
#include <vector>
#include <unordered_map>

std::unique_ptr<std::vector<std::string> > split(std::string str, char delim);
std::string trim(std::string str);
std::unique_ptr<std::unordered_map<std::string,std::string> > read_config(void);

#endif // CONFIG_HPP

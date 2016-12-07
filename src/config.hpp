#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <unordered_map>

std::unique_ptr<std::unordered_map<std::string,std::string> > read_config(void);

#endif // CONFIG_HPP

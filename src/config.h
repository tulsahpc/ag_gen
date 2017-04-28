#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <unordered_map>
#include <string>

class Config {
	std::unordered_map<std::string,std::string> config;
public:
	Config(const std::string &filename);

	void print(void);
	std::string db_string(void);

	std::string operator[](const std::string &q);
};

#endif // CONFIG_HPP

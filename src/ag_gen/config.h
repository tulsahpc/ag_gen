#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_map>

class Config {
    std::unordered_map<std::string, std::string> config;

  public:
    explicit Config(std::string filename);
    std::string operator[](const std::string &q);

    void print();
    std::string db_string();
};

#endif // CONFIG_HPP
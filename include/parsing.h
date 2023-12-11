#pragma once
#include <string>
#include <vector>

#define INCLUDE_STR(Path) Parse::read_file(Path)

namespace Parse {

std::string read_file(const std::string &path);
std::vector<char*> split_str(std::string &&str, const std::string &separator);
std::vector<char*> split_char(char *str, const std::string &separator);

template<typename Fn>
void enum_str(std::string &&str, const std::string &separator, Fn func) {
    enum_char(str.data(), separator, func);
}

template<typename Fn>
void enum_char(char *str, const std::string &separator, Fn func) {
    char *token, *nextt = nullptr;

    token = strtok_s(str, separator.c_str(), &nextt);
    while(token) {
        func(token);
        token = strtok_s(NULL, separator.c_str(), &nextt);
    }
}

}

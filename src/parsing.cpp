#include "parsing.h"

#include <cassert>
#include <fstream>
#include <string.h>

namespace Parse {

std::string read_file(const std::string &path) {
    std::string content = "";

    std::ifstream file(path);
    assert(file.is_open());

    std::string line = "";
    while(std::getline(file, line)) {
        content.append(line);
        content.append("\n");
    }

    return content;
}

std::vector<char*> split_str(std::string &&str, const std::string &separator) {
    return split_char(str.data(), separator);
}

std::vector<char*> split_char(char *str, const std::string &separator) {
    std::vector<char*> tokens;
    char *found = str;

    while((found = strstr(str, separator.c_str()))) {
        tokens.push_back(str);
        str = found;
        for (int i = 0; i < separator.size(); i++) {
            found[i] = '\0';
            str++;
        }
    }

    tokens.push_back(str);

    auto end = std::remove_if(tokens.begin(), tokens.end(), [](char *val){ return strlen(val) == 0; });
    return { tokens.begin(), end };
}

void split_once(char *str, const std::string &separator, char **first, char **rest) {
    *first = strtok_s(str, separator.c_str(), rest);
    for (int i = 0; i < separator.size() - 1; i++) { (*rest)++; }
}

}

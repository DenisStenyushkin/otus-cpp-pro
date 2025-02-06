#pragma once

#include <string>


struct PrefixException : public std::exception {
    PrefixException(std::string string, size_t prefix_length)
        : string{string}, prefix_length{prefix_length} {}

    std::string string;
    size_t prefix_length;
};

std::string extract_prefix(std::string source, size_t prefix_len) {
    if (prefix_len > source.size()) {
        throw PrefixException{source, prefix_len};
    }

    return source.substr(0, prefix_len);
}

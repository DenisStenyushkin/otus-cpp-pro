#pragma once

#include <string>


std::string extract_prefix(std::string source, size_t prefix_len) {
    return source.substr(0, prefix_len);
}

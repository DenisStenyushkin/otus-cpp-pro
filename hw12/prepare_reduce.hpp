#pragma once

#include <forward_list>
#include <string>
#include <vector>

std::vector<std::forward_list<std::string>> prepare_reduce_data(std::forward_list<std::string>& data, size_t num_blocks) {
    std::vector<std::forward_list<std::string>> result{num_blocks};
    size_t idx = 0;

    for (auto& element: data) {
        result[idx % num_blocks].push_front(std::move(element));
        ++idx;
    }

    return result;
}

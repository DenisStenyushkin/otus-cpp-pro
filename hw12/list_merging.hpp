#pragma once

#include <forward_list>
#include <string>
#include <vector>

std::forward_list<std::string> merge(std::vector<std::forward_list<std::string>> lists) {
    if (lists.size() == 0) {
        return std::forward_list<std::string>{};
    }

    if (lists.size() == 1) {
        return lists[0];
    }

    std::forward_list<std::string> result;
    for (auto& list: lists) {
        for (auto& element: list) {
            result.push_front(element);
        }
    }

    return result;
}

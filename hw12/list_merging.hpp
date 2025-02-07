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

    for (size_t i = 1; i < lists.size(); ++i) {
        lists[0].merge(lists[i]);
    }

    return lists[0];
}

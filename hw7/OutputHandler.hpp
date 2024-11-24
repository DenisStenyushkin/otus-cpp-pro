#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace OutputHandling {

class OutputHandler {
public:
    // TODO: Accept iterator
    virtual void Handle(const std::vector<std::string> output) const = 0;
};

class ConsoleOutputHandler: public OutputHandler {
public:
    void Handle(const std::vector<std::string> output) const {
        for (const auto& element: output) {
            std::cout << element << std::endl;
        }
    }
};

} // namespace OutputHandling

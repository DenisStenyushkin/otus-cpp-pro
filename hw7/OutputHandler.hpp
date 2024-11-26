#pragma once

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "CommandMetadata.h"

namespace OutputHandling {

class OutputHandler {
public:
    // TODO: Accept iterator
    virtual void Handle(const std::vector<CommandProcessing::CommandMetadata>& output) const = 0;
};

class ConsoleOutputHandler: public OutputHandler {
public:
    void Handle(const std::vector<CommandProcessing::CommandMetadata>& output) const {
        for (const auto& element: output) {
            std::cout << element.command << std::endl;
        }
    }
};

class FileOutputHandler: public OutputHandler {
public:
    void Handle(const std::vector<CommandProcessing::CommandMetadata>& output) const {
        if (!output.size()) return;

        auto tc = std::chrono::system_clock::to_time_t(output[0].created_at);
        auto tm = *std::localtime(&tc);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(output[0].created_at.time_since_epoch()) % 1000;

        std::stringstream fname_stream;
        fname_stream << "bulk" << std::put_time(&tm, "%H%M%S") << ms.count() << ".log";
        std::ofstream out_file{fname_stream.str()};

        for (const auto& element: output) {
            out_file << element.command << std::endl;
        }

        out_file.close();
    }
};

} // namespace OutputHandling

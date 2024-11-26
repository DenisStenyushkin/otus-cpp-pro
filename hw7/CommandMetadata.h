#pragma once

#include <string>
#include <chrono>


namespace CommandProcessing {

struct CommandMetadata {
    const std::string command;
    const std::chrono::time_point<std::chrono::system_clock> created_at;

    explicit CommandMetadata(const std::string& command)
        : command{command}, created_at{std::chrono::system_clock::now()} {}
};

} // namespace CommandProcessing

#pragma once

#include <forward_list>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "text_file_splitter.hpp"

class IDataProvider {
public:
    virtual std::forward_list<std::string> get_data() = 0;
    virtual ~IDataProvider() = default;
};

class FileDataProvider : public IDataProvider {
public:
    FileDataProvider(const std::string& file_name, const TextFileSplitter::Range& range) 
    : file_name_{file_name}, range_{range} { }

    // FileDataProvider(const FileDataProvider& other)
    //     : FileDataProvider(other.file_name_, other.range_) {}

    std::forward_list<std::string> get_data() override {
        std::ifstream file{file_name_};
        file.seekg(range_.first);

        auto range_size = range_.second - range_.first;
        std::string full_data(range_size, '\0');
        file.read(full_data.data(), range_size);

        std::stringstream data_stream{full_data};
        std::string line;
        std::forward_list<std::string> result;

        while (std::getline(data_stream, line)) {
            result.push_front(std::move(line));
        }
        
        return result;
    }

private:
    const std::string file_name_;
    const TextFileSplitter::Range range_;
};

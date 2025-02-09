#pragma once

#include <fstream>
#include <string>
#include <vector>


class TextFileSplitter {
public:
    using Range = std::pair<std::ifstream::pos_type, std::ifstream::pos_type>;

    TextFileSplitter(const std::string& file_name) : file_name_{file_name} {}

    std::vector<Range> split(size_t num_blocks) const {
        std::ifstream file_tmp{file_name_, std::ios::in | std::ios::ate};
        std::ifstream::pos_type file_size = file_tmp.tellg();
        file_tmp.close();
        if (file_size == std::ifstream::pos_type(-1))
            throw std::runtime_error("Error while opening the file");

        std::ptrdiff_t block_size = file_size / num_blocks;
        std::vector<Range> result{num_blocks};
        std::ifstream::pos_type prev = 0;
        std::ifstream::pos_type next;

        std::ifstream file{file_name_};

        for (std::size_t i = 0; i < num_blocks; ++i)
        {
            next = (file_size - prev < block_size ? file_size : prev + block_size);
            file.seekg(next);
            while (next < file_size && file.peek() != '\n')
            {
                next += 1;
                file.ignore(1);
            }
            result[i].first = prev;
            result[i].second = next;
            prev = next;
            if (prev != file_size)
                prev += 1;
        }

        file.close();
        return result;
    }

private:
    std::string file_name_;
};

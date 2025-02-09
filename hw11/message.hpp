#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

class Message {
public:
    const static size_t header_length = 4;
    const static size_t max_body_length = 9216; // 9KiB

    Message() : body_length_{0} {
        std::memset(data_, 0, header_length+max_body_length);
    }

    const char* data() const {
        return data_;
    }

    char* data() {
        return data_;
    }

    size_t length() const {
        return header_length + body_length_;
    }

    const char* body() const {
        return data_ + body_length_;
    }

    char* body() {
        return data_ + header_length;
    }

    size_t body_length() const {
        return body_length_;
    }

    void body_length(size_t new_length) {
        body_length_ = new_length;
        if (body_length_ > max_body_length) {
            body_length_ = max_body_length;
        }
    }

    bool decode_header() {
        char header[header_length + 1] = "";
        std::strncat(header, data_, header_length);
        body_length_ = std::atoi(header);
        if (body_length_ > max_body_length) {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header() {
        char header[header_length + 1] = "";
        std::sprintf(header, "%d", static_cast<int>(body_length_));
        std::memcpy(data_, header, header_length);
    }

    void clear() {
        std::memset(data_, 0, header_length+max_body_length);
        body_length_ = 0;
    }

private:
    char data_[header_length + max_body_length];
    size_t body_length_;
};

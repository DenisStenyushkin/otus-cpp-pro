#pragma once

#include <string>

class IChecksumProvider{
public:
    virtual std::string CaculateChecksum(const char * data, size_t size) const = 0;
};

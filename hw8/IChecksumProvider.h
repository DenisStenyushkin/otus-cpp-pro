#pragma once

#include <string>

class IChecksumProvider{
public:
    virtual std::string CaculateChecksum(const char * data, size_t size) = 0;
};

class MockChecksumProvider : public IChecksumProvider {
public:
    std::string CaculateChecksum([[maybe_unused]] const char * data, [[maybe_unused]] size_t size) override {
        return "42";
    }
};

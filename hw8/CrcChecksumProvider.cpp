#include <string>
#include <boost/crc.hpp>
#include "CrcChecksumProvider.h"

namespace hw8 {

std::string CrcChecksumProvider::CaculateChecksum(const char * data, size_t size) {
    boost::crc_32_type result;
    result.process_bytes(data, size);
    return std::to_string(result.checksum());
}

} // namespace hw8

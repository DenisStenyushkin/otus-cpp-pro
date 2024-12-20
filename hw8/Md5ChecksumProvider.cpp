#include <string>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include "Md5ChecksumProvider.h"

namespace hw8 {

std::string Md5ChecksumProvider::CaculateChecksum(const char * data, size_t size) const {
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;
    std::string checksum;

    hash.process_bytes(data, size);
    hash.get_digest(digest);

    const auto intDigest = reinterpret_cast<const int*>(&digest);
    boost::algorithm::hex(intDigest, intDigest + (sizeof(decltype(digest))/sizeof(int)),
                          std::back_inserter(checksum));
    return checksum;
}

} // namespace hw8

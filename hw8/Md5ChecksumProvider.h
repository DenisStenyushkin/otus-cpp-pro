#include <string>
#include "IChecksumProvider.h"

namespace hw8 {

class Md5ChecksumProvider : public IChecksumProvider {
    public:
    std::string CaculateChecksum(const char * data, size_t size) override;
};

} // namespace hw8

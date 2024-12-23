#include <string>
#include "IChecksumProvider.h"

namespace hw8 {

class CrcChecksumProvider : public IChecksumProvider {
    public:
    std::string CaculateChecksum(const char * data, size_t size) const override;
};

} // namespace hw8

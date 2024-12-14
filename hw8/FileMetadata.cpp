#include <fstream>
#include <boost/filesystem.hpp>
#include "IChecksumProvider.h"
#include "FileMetadata.h"

namespace hw8 {

namespace fs = boost::filesystem;

FileMetadata::FileMetadata(fs::path& path, size_t fileBlockSize,
                           IChecksumProvider& checksumProvider)
    : m_path{path}, m_fileBlockSize{fileBlockSize}, m_checksumProvider{checksumProvider},
      m_passedEoF{false} {
    m_size = fs::file_size(m_path);
    m_checkSums = {};
}

size_t FileMetadata::getSize() const {
    return m_size;
}

const FileMetadata::Iterator FileMetadata::cbegin() const {
    if (m_checkSums.size() == 0) {
        readNextBlock();
    }
    
    if (m_checkSums.size() != 0) {
        return Iterator{this, 0};
    }
    
    return cend();
}

const FileMetadata::Iterator FileMetadata::cend() const {
    return Iterator{this, {}};
}

FileMetadata::Iterator FileMetadata::advanceIterator(std::optional<size_t> currentIndex) const {
    if (!currentIndex.has_value()) {
        return cend();
    }

    if (currentIndex.value() < (m_checkSums.size() - 1)) {
        return Iterator{this, ++currentIndex.value()};
    }

    readNextBlock();

    if (currentIndex.value() < (m_checkSums.size() - 1)) {
        return Iterator{this, ++currentIndex.value()};
    }
    
    return cend();
}

void FileMetadata::readNextBlock() const {
    if ((m_size == 0) || m_passedEoF) {
        return;
    }

    std::vector<char> buffer;
    buffer.reserve(m_fileBlockSize);

    memset(buffer.data(), 0, m_fileBlockSize);
    std::ifstream fd(m_path.c_str(), std::ifstream::binary);
    fd.seekg(m_checkSums.size() * m_fileBlockSize);

    fd.read(buffer.data(), m_fileBlockSize);

    if (fd.eof()) {
        m_passedEoF = true;
    }

    m_checkSums.push_back(m_checksumProvider.CaculateChecksum(buffer.data(), m_fileBlockSize));
}

} // namespace hw8
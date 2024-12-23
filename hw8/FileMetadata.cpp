#include <fstream>
#include <boost/filesystem.hpp>
#include "IChecksumProvider.h"
#include "FileMetadata.h"

namespace hw8 {

namespace fs = boost::filesystem;

FileMetadata::FileMetadata(const fs::path& path, size_t fileBlockSize,
                           const IChecksumProvider& checksumProvider)
    : m_path{path}, m_fileBlockSize{fileBlockSize}, m_checksumProvider{checksumProvider},
      m_passedEoF{false} {
    m_size = fs::file_size(m_path);
    m_checkSums = {};
}

size_t FileMetadata::getSize() const {
    return m_size;
}

const fs::path& FileMetadata::getPath() const {
    return m_path;
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

FileMetadata::Iterator::Iterator(const FileMetadata* md, std::optional<size_t> index)
            : m_md{md}, m_index{index} {}

FileMetadata::Iterator::reference FileMetadata::Iterator::operator*() {
    if (!m_index.has_value()) {
        throw std::runtime_error("Invalid iterator");
    }
    return m_md->m_checkSums[m_index.value()];
}

FileMetadata::Iterator::pointer FileMetadata::Iterator::operator->() {
    if (!m_index.has_value()) {
        throw std::runtime_error("Invalid iterator");
    }
    return &(m_md->m_checkSums[m_index.value()]);
}

bool operator==(const FileMetadata::Iterator& it1, const FileMetadata::Iterator& it2) {
    return (it1.m_md == it2.m_md) && (it1.m_index == it2.m_index);
}

bool operator!=(const FileMetadata::Iterator& it1, const FileMetadata::Iterator& it2) {
    return !(it1 == it2);
}

bool compare_equal(FileMetadata md1, FileMetadata md2) {
    if (md1.getSize() != md2.getSize()) {
        return false;
    }

    auto md1_it = md1.cbegin(), md1_end = md1.cend();
    auto md2_it = md2.cbegin(), md2_end = md2.cend();

    while (true)
    {
        if (((md1_it == md1_end) && (md2_it != md2_end)) ||
            ((md1_it != md1_end) && (md2_it == md2_end))) {
            return false;
        }
        else if ((md1_it == md1_end) && (md2_it == md2_end)) {
            break;
        }

        // (md1_it != md1_end) && (md2_it != md2_end)
        if (*md1_it != *md2_it) {
            return false;
        }

        ++md1_it;
        ++md2_it;
    }
    
    return true;
}

} // namespace hw8
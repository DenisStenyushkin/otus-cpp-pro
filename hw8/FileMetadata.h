#pragma once

#include <vector>
#include <string>
#include <optional>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include "IChecksumProvider.h"

namespace hw8
{

namespace fs = boost::filesystem;

class FileMetadata {
public:
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::string;
        using pointer = std::string*;
        using reference = std::string&;

        explicit Iterator(const FileMetadata* md, std::optional<size_t> index);
        
        reference operator*();

        pointer operator->();

        Iterator& operator++() {
            this->m_index = m_md->advanceIterator(m_index).m_index;
            return *this;
        }

        friend bool operator==(const Iterator& it1, const Iterator& it2);

        friend bool operator!=(const Iterator& it1, const Iterator& it2);
    
    private:
        const FileMetadata* m_md;
        std::optional<size_t> m_index;
    };

    explicit FileMetadata(const fs::path& path, size_t fileBlockSize, const IChecksumProvider& checksumProvider);
    size_t getSize() const;
    const fs::path& getPath() const;
    const Iterator cbegin() const;
    const Iterator cend() const;
private:
    Iterator advanceIterator(std::optional<size_t> currentIndex) const;
    void readNextBlock() const;

    const fs::path& m_path;
    size_t m_size;
    size_t m_fileBlockSize;
    const IChecksumProvider& m_checksumProvider;
    mutable std::vector<std::string> m_checkSums;
    mutable bool m_passedEoF;
};

bool compare_equal(FileMetadata md1, FileMetadata md2);

} // namespace hw8

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

        explicit Iterator(const FileMetadata* md, std::optional<size_t> index)
            : m_md{md}, m_index{index} {};
        
        reference operator*() {
            if (!m_index.has_value()) {
                throw std::runtime_error("Invalid iterator");
            }
            return m_md->m_checkSums[m_index.value()];
        }

        pointer operator->() {
            if (!m_index.has_value()) {
                throw std::runtime_error("Invalid iterator");
            }
            return &(m_md->m_checkSums[m_index.value()]);
        }

        Iterator operator++() {
            return m_md->advanceIterator(m_index);
        }

        friend bool operator==(const Iterator& it1, const Iterator& it2) {
            // bool exp = (it1.m_md == it2.m_md) && (it1.m_index == it2.m_index);
            // return exp;
            return (it1.m_md == it2.m_md) && (it1.m_index == it2.m_index);
        }

        friend bool operator!=(const Iterator& it1, const Iterator& it2) {
            return !(it1 == it2);
        }
    
    private:
        const FileMetadata* m_md;
        std::optional<size_t> m_index;
    };

    explicit FileMetadata(fs::path& path, size_t fileBlockSize, IChecksumProvider& checksumProvider);
    size_t getSize() const;
    const Iterator cbegin() const;
    const Iterator cend() const;
private:
    Iterator advanceIterator(std::optional<size_t> currentIndex) const;
    void readNextBlock() const;

    fs::path& m_path;
    size_t m_size;
    size_t m_fileBlockSize;
    IChecksumProvider& m_checksumProvider;
    mutable std::vector<std::string> m_checkSums;
    mutable bool m_passedEoF;
};

} // namespace hw8

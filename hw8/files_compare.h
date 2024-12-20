#pragma once
#include <vector>
#include <boost/filesystem.hpp>
#include "FileMetadata.h"

namespace hw8 {

struct EqualityBin {
    EqualityBin(const FileMetadata* s, std::vector<const FileMetadata*> e)
        : seed{s}, equals{e} {}

    const FileMetadata* seed;
    std::vector<const FileMetadata*> equals;
};

using Equals = std::vector<EqualityBin>;

Equals compare_files(const std::vector<FileMetadata>& files_index);

} // namespace hw8
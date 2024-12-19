#pragma once

#include <vector>
#include <boost/filesystem.hpp>
#include "Parameters.h"

namespace hw8 {

namespace fs = boost::filesystem;

std::vector<fs::path> build_files_index(const std::vector<std::string>& includeDirs,
                                        const std::vector<std::string>& excludeDirs,
                                        bool recursive,
                                        size_t minFileSize,
                                        const std::vector<std::string>& fileNameMasks);

} // namespace hw8
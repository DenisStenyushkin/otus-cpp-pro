#include <algorithm>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include "files_index.h"
#include "Parameters.h"

namespace hw8 {

namespace fs = boost::filesystem;

std::vector<fs::path> build_files_index(const std::vector<std::string>& includeDirs,
                                        const std::vector<std::string>& excludeDirs,
                                        bool recursive,
                                        size_t minFileSize,
                                        const std::vector<std::string>& fileNameMasks) {
    std::vector<fs::path> includePaths, excludePaths;
    includePaths.reserve(includeDirs.size());
    excludePaths.reserve(excludeDirs.size());

    auto str2path = [](const auto str_path) { return fs::path(str_path) };
    std::transform(includeDirs.begin(), includeDirs.end(), std::back_inserter(includePaths), str2path);
    std::transform(excludeDirs.begin(), excludeDirs.end(), std::back_inserter(excludePaths), str2path);
}

std::vector<fs::path> dir_files_index(const fs::path& dir,
                                      const std::vector<fs::path>& excludeDirs,
                                      bool recursive,
                                      size_t minFileSize,
                                      const std::vector<std::string>& fileNameMasks) {
    
}

} // namespace hw8
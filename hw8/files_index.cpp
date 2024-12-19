#include <algorithm>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "files_index.h"
#include "Parameters.h"

namespace hw8 {

namespace fs = boost::filesystem;

template<typename T>
void merge_vectors(std::vector<T>& into, const std::vector<T>& from) {
    into.reserve(into.size() + from.size());
    into.insert(into.end(), from.begin(), from.end());
}

bool elem_in_vector(const std::vector<fs::path>& vector, const fs::path& elem) {
    auto res = std::find_if(vector.begin(), vector.end(),
        [elem](const fs::path& velem) {
            return fs::absolute(fs::relative(elem)) == fs::absolute(fs::relative(velem));
        });
    return res != vector.end();
}

std::vector<fs::path> dir_files_index(const fs::path& dir,
                                      const std::vector<fs::path>& excludeDirs,
                                      bool recursive,
                                      size_t minFileSize,
                                      const std::vector<std::string>& fileNameMasks) {
    std::vector<fs::path> result;

    if (elem_in_vector(excludeDirs, dir)) {
        return result;
    }

    for (const auto& entry: fs::directory_iterator(dir)) {
        if (fs::is_directory(entry) && recursive) {
            std::vector<fs::path> res = dir_files_index(entry.path(), excludeDirs, recursive, minFileSize, fileNameMasks);
            merge_vectors(result, res);
        }
        else if (fs::is_regular_file(entry)) {
            if ((fs::file_size(entry.path()) < minFileSize) ||
                (!fileNameMasks.empty() && (!std::any_of(fileNameMasks.begin(), fileNameMasks.end(), 
                    [&entry](const auto& mask) {
						return boost::regex_match(entry.path().filename().string(), boost::regex{ mask });
					})))) {
                continue;
            }

            result.push_back(entry);
        }
    }

    return result;
}

std::vector<fs::path> build_files_index(const std::vector<std::string>& includeDirs,
                                        const std::vector<std::string>& excludeDirs,
                                        bool recursive,
                                        size_t minFileSize,
                                        const std::vector<std::string>& fileNameMasks) {
    std::vector<fs::path> includePaths, excludePaths;
    includePaths.reserve(includeDirs.size());
    excludePaths.reserve(excludeDirs.size());

    auto str2path = [](const auto str_path) { return fs::path(str_path); };
    std::transform(includeDirs.begin(), includeDirs.end(), std::back_inserter(includePaths), str2path);
    std::transform(excludeDirs.begin(), excludeDirs.end(), std::back_inserter(excludePaths), str2path);

    std::vector<fs::path> result;
    for (const auto& dir: includeDirs) {
        std::vector<fs::path> res = dir_files_index(dir, excludePaths, recursive,
                                                    minFileSize, fileNameMasks);
        merge_vectors(result, res);
    }

    return result;
}

} // namespace hw8
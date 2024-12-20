#include <vector>
#include "files_compare.h"
#include "FileMetadata.h"

namespace hw8 {

Equals compare_files(const std::vector<FileMetadata>& files_index) {
    Equals result;

    for (size_t i = 0; i < files_index.size(); ++i) {
        bool found_equal{false};

        for (auto& e: result) {
            if (compare_equal(*e.seed, files_index[i])) {
                e.equals.push_back(&files_index[i]);
                found_equal = true;
                break;
            }
        }

        if (!found_equal) {
            result.emplace_back(&files_index[i], std::vector<const FileMetadata*>{});
        }
    }

    return result;
}

} // namespace hw8
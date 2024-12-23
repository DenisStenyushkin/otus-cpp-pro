#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "Parameters.h"
#include "CrcChecksumProvider.h"
#include "Md5ChecksumProvider.h"
#include "FileMetadata.h"
#include "files_index.h"
#include "files_compare.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main(int argc, char** argv) {
    hw8::Parameters params{argc, argv};

    IChecksumProvider* csp{nullptr};
    if (params.GetHashAlgorithm() == hw8::HashAlgo::CRC32) {
        csp = new hw8::CrcChecksumProvider{};
    }
    else if (params.GetHashAlgorithm() == hw8::HashAlgo::MD5) {
        csp = new hw8::Md5ChecksumProvider{};
    }
    else {
        throw std::invalid_argument("Unknown hash algorithm");
    }

    auto file_path_list = hw8::build_files_index(params.GetIncludeDirs(),
                                                 params.GetExcludeDirs(),
                                                 params.GetScanRecursive(),
                                                 params.GetMinFileSize(),
                                                 params.GetFileNameMasks());

    std::vector<hw8::FileMetadata> metadata_list;
    std::transform(file_path_list.begin(), file_path_list.end(), std::back_inserter(metadata_list),
                   [csp, params](const auto& path) {
                        return hw8::FileMetadata{path, params.GetBufferSize(), *csp};
                    }
    );

    const auto equality_bins = hw8::compare_files(metadata_list);

    for (const auto& b: equality_bins) {
        if (b.equals.empty()) {
            continue;
        }
        
        std::cout << b.seed->getPath().string() << std::endl;
        for (const auto e: b.equals) {
            std::cout << e->getPath().string() << std::endl;
        }
        std::cout << std::endl;
    }


    // auto r = (fs::absolute(fs::path("./.cmake")) == fs::absolute(fs::path(".cmake")));
    // std::cout << fs::absolute(fs::path("./.cmake")) << std::endl;
    // std::cout << fs::absolute(fs::path(".cmake")) << std::endl;

    // std::cout << fs::absolute(fs::relative(fs::path("./.cmake"))) << std::endl;
    // std::cout << fs::absolute(fs::relative(fs::path(".cmake"))) << std::endl;

    // std::cout << r << std::endl;

    return 0;
}
#pragma once

#include <vector>
#include <boost/filesystem.hpp>
#include "Parameters.h"

namespace hw8 {

namespace fs = boost::filesystem;

std::vector<fs::path> build_files_index(const Parameters& params);

} // namespace hw8
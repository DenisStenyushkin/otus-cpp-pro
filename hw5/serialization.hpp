#pragma once

#include <string>
#include "Document.hpp"

void serialize([[maybe_unused]] const std::string& file_name, [[maybe_unused]] const Document::ExportableState& document) {}

Document::ExportableState deserialize([[maybe_unused]] const std::string& file_name) {
    return Document::ExportableState{};
}

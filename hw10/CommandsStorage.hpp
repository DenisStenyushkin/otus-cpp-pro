#pragma once

#include <vector>
#include "CommandMetadata.h"

namespace CommandProcessing {

    struct CommandsStorage
    {
        std::vector<CommandMetadata> commands;
    };
    

} // namespace CommandProcessing

#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "CommandProcessorState.h"
#include "OutputHandler.hpp"

namespace CommandProcessing {

const std::string START_SCOPE_COMMAND = "{";
const std::string END_SCOPE_COMMAND = "}";

class CommandProcessorState;

class CommandProcessor {
public:    
    explicit CommandProcessor(std::initializer_list<std::shared_ptr<OutputHandling::OutputHandler>> output_handlers);

    void HandleCommand(const std::string& command);
    void ProcessCommands();
    void SwitchTo(std::shared_ptr<CommandProcessorState> newState);

private:
    std::shared_ptr<CommandProcessorState> m_state;
    std::vector<std::shared_ptr<OutputHandling::OutputHandler>> m_output_handlers;
};

}

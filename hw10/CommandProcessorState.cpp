#include "CommandProcessorState.h"
#include "CommandsStorage.hpp"

namespace CommandProcessing {

CommandProcessorState::CommandProcessorState(std::shared_ptr<CommandProcessor> processor,
                                             std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                             std::shared_ptr<CommandsStorage> storage )
    : m_processor{processor}, m_state_fabric{state_fabric}, m_commands{storage} {}

CommandProcessorState::~CommandProcessorState() = default;

std::vector<CommandMetadata> CommandProcessorState::GetCommands() {
    return m_commands->commands;
}

} // namespace CommandProcessing

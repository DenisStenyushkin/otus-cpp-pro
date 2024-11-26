#include <vector>
#include "CommandProcessorState.h"

namespace CommandProcessing {

CommandProcessorState::CommandProcessorState(std::shared_ptr<CommandProcessor> processor,
                                             std::shared_ptr<CommandProcessorStateFabric> state_fabric)
    : m_processor{processor}, m_state_fabric{state_fabric} {}

CommandProcessorState::~CommandProcessorState() = default;

std::vector<CommandMetadata> CommandProcessorState::GetCommands() {
    return m_commands;
}

} // namespace CommandProcessing

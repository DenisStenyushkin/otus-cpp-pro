#include "ScopedInputCommandProcessorState.h"
#include "CommandMetadata.h"
#include "CommandsStorage.hpp"

namespace CommandProcessing {

ScopedInputCommandProcessorState::ScopedInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                                                   const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                                                   const std::shared_ptr<CommandsStorage> storage)
        : CommandProcessorState{processor, state_fabric, storage}, m_num_nested_scopes{0} {}

void ScopedInputCommandProcessorState::EnterState() {
    m_processor.lock()->ProcessCommands();
}

void ScopedInputCommandProcessorState::HandleCommand(const std::string& command) {
    if (command == START_SCOPE_COMMAND) {
        ++m_num_nested_scopes;
    }
    else if (command == END_SCOPE_COMMAND) {
        --m_num_nested_scopes;
        if (m_num_nested_scopes < 0) {
            m_processor.lock()->SwitchTo(m_state_fabric.lock()->MakeBatch());
        }
    } else {
        m_commands->commands.push_back(CommandMetadata{command});
    }
}

void ScopedInputCommandProcessorState::ExitState() {
    m_processor.lock()->ProcessCommands();
}

} // namespace CommandProcessing

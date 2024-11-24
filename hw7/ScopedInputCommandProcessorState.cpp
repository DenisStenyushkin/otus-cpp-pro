#include "ScopedInputCommandProcessorState.h"

namespace CommandProcessing {

ScopedInputCommandProcessorState::ScopedInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                                                  const std::shared_ptr<CommandProcessorStateFabric> state_fabric)
        : CommandProcessorState{processor, state_fabric}, m_num_nested_scopes{0} {}

void ScopedInputCommandProcessorState::EnterState() {
    m_processor->ProcessCommands();
}

void ScopedInputCommandProcessorState::HandleCommand(const std::string& command) {
    if (command == START_SCOPE_COMMAND) {
        ++m_num_nested_scopes;
    }
    else if (command == END_SCOPE_COMMAND) {
        --m_num_nested_scopes;
        if (m_num_nested_scopes < 0) {
            m_processor->SwitchTo(m_state_fabric->MakeBatch());
        }
    } else {
        m_commands.push_back(command);
    }
}

void ScopedInputCommandProcessorState::ExitState() {
    m_processor->ProcessCommands();
}

} // namespace CommandProcessing

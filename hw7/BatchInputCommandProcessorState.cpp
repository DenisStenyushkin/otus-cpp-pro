#include "BatchInputCommandProcessorState.h"

namespace CommandProcessing {

BatchInputCommandProcessorState::BatchInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                            const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                            std::size_t capacity)
    : CommandProcessorState{processor, state_fabric} {
    m_commands.reserve(capacity);
}

void BatchInputCommandProcessorState::EnterState() {
    m_processor->ProcessCommands();
}

void BatchInputCommandProcessorState::HandleCommand(const std::string& command) {
    if (command == START_SCOPE_COMMAND) {
        m_processor->SwitchTo(m_state_fabric->MakeScoped());
        return;
    }

    m_commands.push_back(command);
    if (m_commands.size() == m_commands.capacity()) {
        m_processor->ProcessCommands();
        m_commands.clear();
    }
}

void BatchInputCommandProcessorState::ExitState() {
    m_processor->ProcessCommands();
}

}
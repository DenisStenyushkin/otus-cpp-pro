#include "BatchInputCommandProcessorState.h"
#include "CommandMetadata.h"
#include "CommandsStorage.hpp"

namespace CommandProcessing {

BatchInputCommandProcessorState::BatchInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                            const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                            std::size_t capacity,
                                            std::shared_ptr<CommandsStorage> storage)
    : CommandProcessorState{processor, state_fabric, storage} {
    m_commands->commands.reserve(capacity);
}

void BatchInputCommandProcessorState::EnterState() {
    m_processor.lock()->ProcessCommands();
}

void BatchInputCommandProcessorState::HandleCommand(const std::string& command) {
    if (command == START_SCOPE_COMMAND) {
        m_processor.lock()->SwitchTo(m_state_fabric.lock()->MakeScoped());
        return;
    }

    m_commands->commands.push_back(CommandMetadata{command});
    if (m_commands->commands.size() == m_commands->commands.capacity()) {
        m_processor.lock()->ProcessCommands();
        m_commands->commands.clear();
    }
}

void BatchInputCommandProcessorState::ExitState() {
    m_processor.lock()->ProcessCommands();
    m_commands->commands.clear();
}

} // namespace CommandProcessing

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "CommandProcessor.h"
#include "OutputHandler.hpp"

namespace CommandProcessing {

CommandProcessorState::CommandProcessorState(std::shared_ptr<CommandProcessor> processor,
                                             std::shared_ptr<CommandProcessorStateFabric> state_fabric)
    : m_processor{processor}, m_state_fabric{state_fabric} {}

CommandProcessorState::~CommandProcessorState() = default;

std::vector<std::string> CommandProcessorState::GetCommands() {
    return m_commands;
}

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

CommandProcessorStateFabric::CommandProcessorStateFabric(std::shared_ptr<CommandProcessor> processor,
                                                         std::size_t batch_capacity)
        : m_processor{processor}, m_batch_capacity{batch_capacity} {}
    
std::shared_ptr<CommandProcessorState> CommandProcessorStateFabric::MakeBatch() {
    return std::make_shared<BatchInputCommandProcessorState>(m_processor, shared_from_this(),
        m_batch_capacity
    );
}

std::shared_ptr<CommandProcessorState> CommandProcessorStateFabric::MakeScoped() {
    return std::make_shared<ScopedInputCommandProcessorState>(m_processor, shared_from_this());
}

CommandProcessor::CommandProcessor(std::initializer_list<std::shared_ptr<OutputHandling::OutputHandler>> output_handlers)
    : m_state{nullptr}, m_output_handlers{output_handlers} {}

void CommandProcessor::HandleCommand(const std::string& command) {
    m_state->HandleCommand(command);
}

void CommandProcessor::ProcessCommands() {
        for (const auto& handler: m_output_handlers) {
            handler->Handle(m_state->GetCommands());
        }
}

void CommandProcessor::SwitchTo(std::shared_ptr<CommandProcessorState> newState) {
    if (m_state != nullptr) {
        m_state->ExitState();
    }
    m_state = newState;
    m_state->EnterState();
}

} // namespace CommandProcessing
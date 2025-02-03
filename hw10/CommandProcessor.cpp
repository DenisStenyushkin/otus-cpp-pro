#include "CommandProcessor.h"

namespace CommandProcessing {

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

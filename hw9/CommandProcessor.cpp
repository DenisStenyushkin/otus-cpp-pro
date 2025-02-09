#include "CommandProcessor.h"

namespace CommandProcessing {

CommandProcessor::CommandProcessor(OutputHandlerFn output_handler)
    : m_state{nullptr}, m_output_handler{output_handler} {}

void CommandProcessor::HandleCommand(const std::string& command) {
    m_state->HandleCommand(command);
}

void CommandProcessor::ProcessCommands() {
    auto commands = m_state->GetCommands();
    m_output_handler(commands);
}

void CommandProcessor::SwitchTo(std::shared_ptr<CommandProcessorState> newState) {
    if (m_state != nullptr) {
        m_state->ExitState();
    }
    m_state = newState;
    m_state->EnterState();
}

} // namespace CommandProcessing

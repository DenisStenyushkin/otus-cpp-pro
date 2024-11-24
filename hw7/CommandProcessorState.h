#pragma once

#include <memory>
#include <vector>

#include "CommandProcessor.h"
#include "CommandProcessorStateFabric.h"

namespace CommandProcessing {

class CommandProcessor;
class CommandProcessorStateFabric;

class CommandProcessorState {
public:
    explicit CommandProcessorState(std::shared_ptr<CommandProcessor> processor,
                                   std::shared_ptr<CommandProcessorStateFabric> state_fabric);

    virtual ~CommandProcessorState();

    virtual void EnterState() = 0;
    virtual void HandleCommand(const std::string& command) = 0;
    virtual void ExitState() = 0;

    // TODO: const vector of const strings
    virtual std::vector<std::string> GetCommands();

protected:
    std::shared_ptr<CommandProcessor> m_processor;
    std::shared_ptr<CommandProcessorStateFabric> m_state_fabric;
    std::vector<std::string> m_commands;
};

} // namespace CommandProcessing
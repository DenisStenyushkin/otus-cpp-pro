#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "OutputHandler.hpp"

namespace CommandProcessing {

const std::string START_SCOPE_COMMAND = "{";
const std::string END_SCOPE_COMMAND = "}";

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

class BatchInputCommandProcessorState: public CommandProcessorState {
public:
    explicit BatchInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                             const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                             std::size_t capacity);

    void EnterState() override;
    void HandleCommand(const std::string& command) override;
    void ExitState() override;
};

class ScopedInputCommandProcessorState: public CommandProcessorState {
public:
    explicit ScopedInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                              const std::shared_ptr<CommandProcessorStateFabric> state_fabric);

    void EnterState() override;
    void HandleCommand(const std::string& command) override;
    void ExitState() override;

private:
    int8_t m_num_nested_scopes;
};

class CommandProcessorStateFabric: public std::enable_shared_from_this<CommandProcessorStateFabric> {
public:
    explicit CommandProcessorStateFabric(std::shared_ptr<CommandProcessor> processor,
                                         std::size_t batch_capacity);
    
    std::shared_ptr<CommandProcessorState> MakeBatch();
    std::shared_ptr<CommandProcessorState> MakeScoped();

private:
    std::shared_ptr<CommandProcessor> m_processor;
    std::size_t m_batch_capacity;
};

class CommandProcessor {
public:    
    explicit CommandProcessor(std::initializer_list<std::shared_ptr<OutputHandling::OutputHandler>> output_handlers);

    void HandleCommand(const std::string& command);

    void ProcessCommands();

    void SwitchTo(std::shared_ptr<CommandProcessorState> newState);

private:
    std::shared_ptr<CommandProcessorState> m_state;
    std::vector<std::shared_ptr<OutputHandling::OutputHandler>> m_output_handlers;
};

} // namespace CommandProcessing
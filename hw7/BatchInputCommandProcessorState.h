#pragma once

#include <memory>
#include <string>

#include "CommandProcessor.h"
#include "CommandProcessorState.h"
#include "CommandProcessorStateFabric.h"

namespace CommandProcessing {

class BatchInputCommandProcessorState: public CommandProcessorState {
public:
    explicit BatchInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                             const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                             std::size_t capacity);

    void EnterState() override;
    void HandleCommand(const std::string& command) override;
    void ExitState() override;
};

} // namespace CommandProcessing

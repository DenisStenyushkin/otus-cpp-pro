#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "CommandProcessor.h"
#include "CommandProcessorState.h"
#include "CommandProcessorStateFabric.h"

namespace CommandProcessing {

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

} // namespace CommandProcessing

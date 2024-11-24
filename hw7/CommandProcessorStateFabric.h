#pragma once

#include <memory>

#include "CommandProcessor.h"
#include "CommandProcessorState.h"

namespace CommandProcessing {

class CommandProcessor;
class CommandProcessorState;

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

} // namespace CommandProcessing

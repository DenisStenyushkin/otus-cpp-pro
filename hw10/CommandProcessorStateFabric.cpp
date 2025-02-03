#include <memory>

#include "CommandProcessorStateFabric.h"
#include "BatchInputCommandProcessorState.h"
#include "ScopedInputCommandProcessorState.h"

namespace CommandProcessing {

CommandProcessorStateFabric::CommandProcessorStateFabric(std::shared_ptr<CommandProcessor> processor,
                                                         std::size_t batch_capacity)
        : m_processor{processor}, m_batch_capacity{batch_capacity} { }
    
std::shared_ptr<CommandProcessorState> CommandProcessorStateFabric::MakeBatch() {
    if (!m_shared_batch_state) {
        m_shared_batch_state = std::make_shared<BatchInputCommandProcessorState>(m_processor, shared_from_this(),
                                                                             m_batch_capacity);
    }

    return m_shared_batch_state;
}

std::shared_ptr<CommandProcessorState> CommandProcessorStateFabric::MakeScoped() {
    return std::make_shared<ScopedInputCommandProcessorState>(m_processor, shared_from_this());
}

} // namespace CommandProcessing

#include <memory>

#include "CommandProcessorStateFabric.h"
#include "CommandsStorage.hpp"
#include "BatchInputCommandProcessorState.h"
#include "ScopedInputCommandProcessorState.h"

namespace CommandProcessing {

CommandProcessorStateFabric::CommandProcessorStateFabric(std::shared_ptr<CommandProcessor> processor,
                                                         std::size_t batch_capacity,
                                                         std::shared_ptr<CommandsStorage> batch_storage)
        : m_processor{processor}, m_batch_capacity{batch_capacity}, m_batch_storage{batch_storage} { }

std::shared_ptr<CommandProcessorState> CommandProcessorStateFabric::MakeBatch() {
    return std::make_shared<BatchInputCommandProcessorState>(m_processor, shared_from_this(),
                                                             m_batch_capacity,
                                                             m_batch_storage);
}

std::shared_ptr<CommandProcessorState> CommandProcessorStateFabric::MakeScoped() {
    return std::make_shared<ScopedInputCommandProcessorState>(m_processor, shared_from_this(),
                                                              std::make_shared<CommandsStorage>());
}

} // namespace CommandProcessing

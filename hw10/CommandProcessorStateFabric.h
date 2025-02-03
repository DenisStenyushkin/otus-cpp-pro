#pragma once

#include <memory>

#include "CommandProcessor.h"
#include "CommandProcessorState.h"

namespace CommandProcessing {

class CommandProcessor;
class CommandProcessorState;

/**
 * Фабрика состояний процессора команд
 */
class CommandProcessorStateFabric: public std::enable_shared_from_this<CommandProcessorStateFabric> {
public:
    /**
     * Конструктор
     * 
     * @param processor Процессор команд, для которого будут создаваться состояния
     * @param batch_capacity Размер пакета команд для состояний, обрабатывающих пакеты фиксированного размера
     */
    explicit CommandProcessorStateFabric(std::shared_ptr<CommandProcessor> processor,
                                         std::size_t batch_capacity);
    
    /**
     * Создает экземпляр состояния пакетной обработки команд
     * 
     * @return Объект состояния
     */
    std::shared_ptr<CommandProcessorState> MakeBatch();

    /**
     * Создает экземпляр состояния обработки пакетов динамического размера
     * 
     * @return Объект состояния
     */
    std::shared_ptr<CommandProcessorState> MakeScoped();

private:
    std::shared_ptr<CommandProcessor> m_processor;
    std::size_t m_batch_capacity;
    std::shared_ptr<CommandProcessorState> m_shared_batch_state;
};

} // namespace CommandProcessing

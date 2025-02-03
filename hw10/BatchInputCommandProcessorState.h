#pragma once

#include <memory>
#include <string>

#include "CommandProcessor.h"
#include "CommandProcessorState.h"
#include "CommandProcessorStateFabric.h"

namespace CommandProcessing {

/**
 * @brief Состояние пакетной обработки команд.
 * 
 * Состояние процессора команд, когда он обрабатывает команды пакетами фиксированного размера.
 * Размер обраюатываемого пакета определяется параметром конструктора.
 */
class BatchInputCommandProcessorState: public CommandProcessorState {
public:
    /**
     * Конструктор
     * 
     * @param processor Указатель на процессор команд, к которому относится состояние
     * @param state_fabric Указатель на фабрику, создающую новые состояния
     * @param capacity Размер обрабатываемого пакета команд
     * @param storage Внешнее хранилище комманд
     */
    explicit BatchInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                             const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                             std::size_t capacity,
                                             std::shared_ptr<CommandsStorage> storage);

    /**
     * Операция, выполняемая при входе в состояние
     */
    void EnterState() override;

    /**
     * Обрабатывает очередную поступившую команду
     * 
     * @param command Поступившая команда
     */
    void HandleCommand(const std::string& command) override;

    /**
     * Операция, выполняемая при выходе из состояния
     */
    void ExitState() override;

    /**
     * Деструктор
     */
    ~BatchInputCommandProcessorState();
};

} // namespace CommandProcessing

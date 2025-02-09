#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "CommandProcessor.h"
#include "CommandProcessorState.h"
#include "CommandProcessorStateFabric.h"

namespace CommandProcessing {

/**
 * @brief Состояние обработки пакетов команд динамического размера
 * 
 * Состояние процессора команд, когда он обрабатывает команды пакетами динамического размера.
 * Границы пакета определяются специальными символами.
 */
class ScopedInputCommandProcessorState: public CommandProcessorState {
public:
    /**
     * Конструктор
     * 
     * @param processor Указатель на процессор команд, к которому относится состояние
     * @param state_fabric Указатель на фабрику, создающую новые состояния
     * @param storage Внешнее хранилище комманд
     */
    explicit ScopedInputCommandProcessorState(const std::shared_ptr<CommandProcessor> processor,
                                              const std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                              const std::shared_ptr<CommandsStorage> storage);

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

private:
    int8_t m_num_nested_scopes;
};

} // namespace CommandProcessing

#pragma once

#include <memory>
#include <vector>

#include "CommandMetadata.h"
#include "CommandProcessor.h"
#include "CommandProcessorStateFabric.h"
#include "CommandsStorage.hpp"

namespace CommandProcessing {

class CommandProcessor;
class CommandProcessorStateFabric;

/**
 * Базовый чисто виртуальный базовый класс для состояний процессора команд 
 */
class CommandProcessorState {
public:
    /**
     * Конструктор
     * 
     * @param processor Процессор команд, к которому относится состояние
     * @param state_fabric Экземпляр фабрики для создания новых состояний
     * @param storage Внешнее хранилище комманд
     */
    explicit CommandProcessorState(std::shared_ptr<CommandProcessor> processor,
                                   std::shared_ptr<CommandProcessorStateFabric> state_fabric,
                                   std::shared_ptr<CommandsStorage> storage);

    /**
     * Деструктор
     */
    virtual ~CommandProcessorState();

    /**
     * Операция, выполняемая при входе в состояние
     */
    virtual void EnterState() = 0;

    /**
     * Обрабатывает очередную поступившую команду
     * 
     * @param command Поступившая команда
     */
    virtual void HandleCommand(const std::string& command) = 0;

    /**
     * Операция, выполняемая при выходе из состояния
     */
    virtual void ExitState() = 0;

    /**
     * Возвращает метаданные команд, входящих в текущий пакет
     * 
     * @return Список объектов метаданных по одному на каждую команду 
     */
    virtual std::vector<CommandMetadata> GetCommands();

protected:
    std::weak_ptr<CommandProcessor> m_processor; ///< Процессор команд
    std::weak_ptr<CommandProcessorStateFabric> m_state_fabric; ///< Фабрика новых состояний
    std::shared_ptr<CommandsStorage> m_commands; ///< Команды текущего пакета
};

} // namespace CommandProcessing

#pragma once

#include <memory>
#include <vector>

#include "ContextManager.hpp"
#include "CommandMetadata.h"
#include "CommandProcessor.h"
#include "CommandProcessorStateFabric.h"

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
     */
    explicit CommandProcessorState(std::shared_ptr<CommandProcessor> processor,
                                   std::shared_ptr<CommandProcessorStateFabric> state_fabric);

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
    std::shared_ptr<CommandProcessor> m_processor; ///< Процессор команд
    std::shared_ptr<CommandProcessorStateFabric> m_state_fabric; ///< Фабрика новых состояний
    std::vector<CommandMetadata> m_commands; ///< Команды текущего пакета
};

} // namespace CommandProcessing

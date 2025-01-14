#pragma once

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "CommandMetadata.h"
#include "CommandProcessorState.h"

namespace CommandProcessing {

const std::string START_SCOPE_COMMAND = "{"; ///< Символ, распознаваемый как маркер начала пакета команд динамического размера
const std::string END_SCOPE_COMMAND = "}"; ///< Символ, распознаваемый как маркер окончания пакета команд динамического размера

class CommandProcessorState;

/**
 * Процессор команд. Реализован как машина состояний с состояниями-объектам. Обработка поступающих команд передается текущему
 * состоянию. За переход между состояниями отвечают сами состояния или внешний код.
 */
class CommandProcessor {
public:
    using OutputHandlerFn = std::function<void(std::vector<CommandProcessing::CommandMetadata>&)>;

    /**
     * Конструктор
     * 
     * @param output_handler Функция, осуществляющая вывод пакета команд по необходимости
     * 
     * @warning Процессор создается с пустым начальным состоянием и требует явной его установки с помощью вызова SwithTo
     */
    explicit CommandProcessor(OutputHandlerFn output_handler);

    /**
     * Обрабатывает поступающую команду
     * 
     * @param command Текст команды
     */
    void HandleCommand(const std::string& command);

    /**
     * Обрабатывает вывод команд текущего пакета по запросу состояния
     */
    void ProcessCommands();

    /**
     * Инициирует переход в новое состояние
     * 
     * @param newState Новый объект состояние. Переход вызывает метод ExitState текущего состояния и 
     * метод EnterState нового
     */
    void SwitchTo(std::shared_ptr<CommandProcessorState> newState);

private:
    std::shared_ptr<CommandProcessorState> m_state;
    OutputHandlerFn m_output_handler;
};

} // namespace CommandProcessing

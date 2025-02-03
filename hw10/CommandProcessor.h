#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "CommandProcessorState.h"
#include "OutputHandler.hpp"

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
    /**
     * Конструктор
     * 
     * @param output_handlers Список объектов, осуществляющих вывод пакета команд по необходимости
     * 
     * @warning Процессор создается с пустым начальным состоянием и требует явной его установки с помощью вызова SwithTo
     */
    explicit CommandProcessor(std::initializer_list<std::shared_ptr<OutputHandling::OutputHandler>> output_handlers);

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
    std::vector<std::shared_ptr<OutputHandling::OutputHandler>> m_output_handlers;
};

} // namespace CommandProcessing

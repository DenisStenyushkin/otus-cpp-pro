#pragma once

#include <string>
#include <chrono>


namespace CommandProcessing {

/**
 * Описывает метаданные команды
 */
struct CommandMetadata {
    std::string command; ///< Текст команды
    std::chrono::time_point<std::chrono::system_clock> created_at; ///< Дата и время создания команды

    /**
     * Конструктор
     * 
     * @param command Текст команды
     */
    explicit CommandMetadata(const std::string& command)
        : command{command}, created_at{std::chrono::system_clock::now()} {}
};

} // namespace CommandProcessing

#pragma once

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "CommandMetadata.h"

namespace OutputHandling {

/**
 * Чисто виртуальный базовый класс для обработчиков вывода процессора команд
 */
class OutputHandler {
public:
    /**
     * Обрабатывает вывод пакета команд
     * 
     * @param output Список метаданных команд для вывода
     */
    virtual void Handle(const std::vector<CommandProcessing::CommandMetadata>& output) const = 0;
};

/**
 * Реализует вывод пакета команд в консоль
 */
class ConsoleOutputHandler: public OutputHandler {
public:
/**
     * Обрабатывает вывод пакета команд
     * 
     * @param output Список метаданных команд для вывода
     */
    void Handle(const std::vector<CommandProcessing::CommandMetadata>& output) const {
        if (! output.size()) {
            return;
        }

        std::cout << "bulk: ";
        for (size_t i = 0; i < output.size(); ++i) {
            std::cout << output[i].command;
            if (i < (output.size() - 1)) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
};

/**
 * Реализует вывод пакета команд в текстовый файл
 */
class FileOutputHandler: public OutputHandler {
private:
    std::string get_random_string(size_t size) const {
        std::string bucket = "abcdefghijklmnopqrstuvwxyz";
        std::string uuid;
        for (size_t i = 0; i < size; ++i) {
            uuid += bucket[rand() % bucket.size()];
        }
        return uuid;
    }

public:
/**
     * Обрабатывает вывод пакета команд
     * 
     * @param output Список метаданных команд для вывода
     */
    void Handle(const std::vector<CommandProcessing::CommandMetadata>& output) const {
        if (!output.size()) return;

        auto tc = std::chrono::system_clock::to_time_t(output[0].created_at);
            auto tm = *std::localtime(&tc);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(output[0].created_at.time_since_epoch()) % 1000;

            std::stringstream fname_stream;
            fname_stream << "bulk" << std::put_time(&tm, "%H%M%S") << ms.count() << "(" + get_random_string(5) + ")" << ".log";
            std::ofstream out_file{fname_stream.str()};

            out_file << "bulk: ";
            for (size_t i = 0; i < output.size(); ++i) {
                out_file << output[i].command;
                if (i < (output.size() - 1)) {
                    out_file << ", ";
                }
            }
            out_file << std::endl;

            out_file.close();
    }
};

} // namespace OutputHandling

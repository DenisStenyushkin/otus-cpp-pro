#pragma once

#include <iomanip>
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

#include "async.h"
#include "CommandMetadata.h"
#include "CommandProcessor.h"
#include "WaitingQueue.hpp"

namespace hw9 {

class ContextManager {
public:
    using WorkerThreadPool = std::vector<std::thread>;

    ContextManager(size_t num_file_threads) {
        m_console_thread = std::thread{&ContextManager::console_worker, this};

        m_file_threads = WorkerThreadPool{};
        m_file_threads.reserve(num_file_threads);
        for (size_t i = 0; i < num_file_threads; ++i) {
            m_file_threads.emplace_back(&ContextManager::file_worker, this);
        };
    }

    ~ContextManager() {
        m_console_thread.join();
        for (auto& t: m_file_threads) {
            t.join();
        }
    }

    ContextID create_new_context(size_t batch_capacity) {
        ContextID new_ctx_id = m_context_map.size() + 1; // ctx_id start from 1

        auto processor = std::make_shared<CommandProcessing::CommandProcessor>(
            std::bind(&ContextManager::output_callback, this, std::placeholders::_1)
        );
        auto processor_state_fabric = std::make_shared<CommandProcessing::CommandProcessorStateFabric>(
            processor, batch_capacity
        );
        processor->SwitchTo(processor_state_fabric->MakeBatch());

        m_context_map.emplace(new_ctx_id, processor);
        
        return new_ctx_id;
    }

    void remove_context(ContextID ctx_id) {
        for (auto it = m_context_map.begin(), last = m_context_map.end(); it != last;) {
            if (it->first == ctx_id) {
                it = m_context_map.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void handle_command(ContextID ctx_id, const std::string& command) {
        m_context_map.at(ctx_id)->HandleCommand(command);
    }

    void stop() {
        m_console_data_queue.stop();
        m_file_data_queue.stop();
    }
private:
    using CommandMetadataVector = std::vector<CommandProcessing::CommandMetadata>;

    std::unordered_map<ContextID, std::shared_ptr<CommandProcessing::CommandProcessor>> m_context_map;
    std::thread m_console_thread;
    WorkerThreadPool m_file_threads;
    WaitingQueue<CommandMetadataVector> m_console_data_queue;
    WaitingQueue<CommandMetadataVector> m_file_data_queue;

    void console_worker() {
        CommandMetadataVector output;

        while (m_console_data_queue.pop(output)) {
            if (!output.size()) {
                continue;
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
        
    }

    void file_worker() {
        CommandMetadataVector output;

        while (m_file_data_queue.pop(output)) {
            if (!output.size()) {
                continue;
            }

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
    }

    void output_callback(CommandMetadataVector& commands) {
        CommandMetadataVector commands_copy = commands;

        m_console_data_queue.push(commands);
        m_file_data_queue.push(commands_copy);
    }

    std::string get_random_string(size_t size) {
        std::string bucket = "abcdefghijklmnopqrstuvwxyz";
        std::string uuid;
        for (size_t i = 0; i < size; ++i) {
            uuid += bucket[rand() % bucket.size()];
        }
        return uuid;
    }
};

} // namespace hw9

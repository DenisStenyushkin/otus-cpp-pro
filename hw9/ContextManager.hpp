#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
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

    void file_worker() {}

    void output_callback(CommandMetadataVector& commands) {
        m_console_data_queue.push(commands);
        m_file_data_queue.push(commands);
    }
};

} // namespace hw9

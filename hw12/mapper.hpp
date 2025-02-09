#pragma once

#include <forward_list>
#include <functional>
#include <future>
#include <string>
#include <vector>

#include "data_provider.hpp"


class Mapper {
public:
    Mapper(size_t num_threads, const std::vector<IDataProvider*>& data_providers,
           std::function<std::string(std::string)> map_function)
        : num_threads_{num_threads}, data_providers_{data_providers},
          map_function_{map_function} {
        if (num_threads != data_providers.size()) {
            throw std::runtime_error("data_providers.size() != num_threads");
        }
    }
    
    std::vector<std::forward_list<std::string>> run() {
        std::vector<std::future<std::forward_list<std::string>>> jobs;
        for (size_t i = 0; i < num_threads_; ++i) {
            jobs.push_back(std::async(std::launch::async,
                [this, i]() {
                    std::forward_list<std::string> result;
                    
                    for (auto line: data_providers_[i]->get_data()) {
                        result.push_front(map_function_(line));
                    }

                    return result;
                }));
        }

        std::vector<std::forward_list<std::string>> result;
        for (auto& job: jobs) {
            result.push_back(job.get());
        }

        return result;
    }

private:
    size_t num_threads_;
    std::vector<IDataProvider*> data_providers_;
    std::function<std::string(std::string)> map_function_;
};

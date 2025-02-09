#pragma once

#include <forward_list>
#include <functional>
#include <future>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Reducer {
public:
    using reduce_input_t = std::forward_list<std::string>;
    using reduce_result_t = std::unordered_map<std::string, size_t>;
    using reduce_fn_t = std::function<reduce_result_t(reduce_input_t::iterator,
                                                      reduce_input_t::iterator)>;

    Reducer(size_t num_threads, std::vector<reduce_input_t>& data,
            reduce_fn_t reduce_function)
        : num_threads_{num_threads}, data_{data}, reduce_function_{reduce_function} {
        if (num_threads != data.size()) {
            throw std::runtime_error("data.size() != num_threads");
        }
    }

    reduce_result_t run() {
        std::vector<std::future<reduce_result_t>> jobs;
        for (size_t i = 0; i < num_threads_; ++i) {
            jobs.push_back(std::async(std::launch::async,
                [this, i]() {
                    return reduce_function_(data_[i].begin(), data_[i].end());
                }));
        }

        std::vector<reduce_result_t> job_results;
        for (auto& job: jobs) {
            job_results.push_back(job.get());
        }

        reduce_result_t result;
        for (auto& job_result: job_results) {
            for (const auto& [k, v]: job_result) {
                result[k] += v;
            }
        }

        return result;
    }

private:
    size_t num_threads_;
    std::vector<reduce_input_t>& data_;
    reduce_fn_t reduce_function_;
};

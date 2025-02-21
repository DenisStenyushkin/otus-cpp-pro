#include <algorithm>
#include <iostream>
#include <functional>
#include <string>

#include "data_provider.hpp"
#include "list_merging.hpp"
#include "mapper.hpp"
#include "map_func.hpp"
#include "prepare_reduce.hpp"
#include "reducer.hpp"
#include "reduce_fn.hpp"
#include "text_file_splitter.hpp"


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: mapreduce <filename> <n_mappers> <n_reducers>\n";
        return 1;
    }

    std::string file_name{argv[1]};
    size_t n_mappers{static_cast<size_t>(std::stoi(argv[2]))};
    size_t n_reducers{static_cast<size_t>(std::stoi(argv[3]))};

    TextFileSplitter splitter{file_name};
    auto ranges = splitter.split(n_mappers);

    std::vector<FileDataProvider> data_providers;
    for (size_t i = 0; i < n_mappers; ++i) {
        data_providers.emplace_back(file_name, ranges[i]);
    }
    std::vector<IDataProvider*> data_provider_pointers;
    std::transform(data_providers.begin(),
                   data_providers.end(),
                   std::back_inserter(data_provider_pointers),
                   [](auto& p) { return &p; });
    
    const size_t max_prefix_len{20};
    std::string prefix;

    for (size_t prefix_len = 1; prefix_len <= max_prefix_len; ++prefix_len) {
        Mapper mapper{n_mappers, data_provider_pointers,
                    std::bind(extract_prefix, std::placeholders::_1, prefix_len)};

        auto map_result = mapper.run();
        auto merge_result = merge(map_result);
        auto reduce_input = prepare_reduce_data(merge_result, n_reducers);

        Reducer reducer{n_reducers, reduce_input, count};
        auto reduce_result = reducer.run();

        if (reduce_result.size() == 1) {
            prefix = reduce_result.begin()->first;
        } else {
            break;
        }
    }

    if (!prefix.empty()) {
        std::cout << "Found minimum common prefix: " << prefix << std::endl;
    }
    else {
        std::cout << "Unable to find minimum common prefix." << std::endl;
    }
}

#include <algorithm>
#include <iostream>
#include <functional>
#include <string>

#include "data_provider.hpp"
#include "list_merging.hpp"
#include "mapper.hpp"
#include "map_func.hpp"
#include "text_file_splitter.hpp"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: mapreduce <filename> <n_mappers>\n";
        return 1;
    }

    std::string file_name{argv[1]};
    size_t n_mappers{static_cast<size_t>(std::stoi(argv[2]))};

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

    using namespace std::placeholders;
    Mapper mapper{n_mappers, data_provider_pointers, std::bind(extract_prefix, _1, 3)};

    auto map_result = mapper.run();
    for (auto& res: map_result) {
        res.sort();
    }
    auto merge_result = merge(map_result);
}

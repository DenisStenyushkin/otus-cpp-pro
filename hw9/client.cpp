#include <iostream>
#include <string>

#include "async.h"


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "The program accepts exactly one integral argument" << std::endl;
        std::cout << "Example: ./bulk 3" << std::endl;
        hw9::close_async();

        return -1;
    }

    size_t batch_size = std::stoi(argv[1]);
    hw9::ContextID ctx_id = hw9::connect(batch_size);
        
    for(std::string line; std::getline(std::cin, line);)
    {
        hw9::receive(ctx_id, line);
    }

    hw9::disconnect(ctx_id);
    hw9::close_async();
}

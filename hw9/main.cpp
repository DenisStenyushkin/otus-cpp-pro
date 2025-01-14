#include <iostream>
#include <string>
#include <thread>

#include "async.h"


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "The program accepts exactly one integral argument" << std::endl;
        std::cout << "Example: ./bulk 3" << std::endl;
        return -1;
    }

    size_t batch_size = std::stoi(argv[1]);

    std::thread sender1{[batch_size]() {
        hw9::ContextID ctx_id = hw9::connect(batch_size);
        
        // ...

        hw9::disconnect(ctx_id);
    }};
}

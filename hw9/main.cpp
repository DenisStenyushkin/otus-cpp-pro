#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "async.h"


using namespace std::chrono_literals;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "The program accepts exactly one integral argument" << std::endl;
        std::cout << "Example: ./bulk 3" << std::endl;
        return -1;
    }

    size_t batch_size = std::stoi(argv[1]);

    std::thread sender1{[batch_size]() {
        hw9::ContextID ctx_id = hw9::connect(batch_size);
        
        hw9::receive(ctx_id, "cmd1");
        hw9::receive(ctx_id, "cmd2");
        hw9::receive(ctx_id, "cmd3");

        hw9::disconnect(ctx_id);
    }};

    std::thread sender2{[batch_size]() {
        hw9::ContextID ctx_id = hw9::connect(batch_size);
        
        hw9::receive(ctx_id, "{");
        hw9::receive(ctx_id, "cmd1");
        hw9::receive(ctx_id, "cmd2");
        hw9::receive(ctx_id, "}");

        hw9::disconnect(ctx_id);
    }};

    sender1.join();
    sender2.join();

    std::this_thread::sleep_for(2s);

    hw9::close_async();
}

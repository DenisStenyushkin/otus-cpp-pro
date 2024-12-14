#include <iostream>
#include "Parameters.h"
#include "IChecksumProvider.h"
#include "FileMetadata.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main(int argc, char** argv) {
    hw8::Parameters params{argc, argv};
    // std::cout << params << std::endl;

    MockChecksumProvider csp;
    fs::path p{"txt1.txt"};
    hw8::FileMetadata md{p, 3, csp};

    std::cout << md.getSize() << std::endl;

    // for (auto it = md.cbegin(); it != md.cend(); ++it) {
    //     std::cout << *it << std::endl;
    // }

    auto it = md.cbegin();
    auto it1_end = md.cend();

    while (true) {
        std::cout << *it << std::endl;
        it = ++it;
        if (it == it1_end) {
            break;
        }
    }

    return 0;
}
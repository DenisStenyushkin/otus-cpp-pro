#include <iostream>
#include "Parameters.h"
#include "Md5ChecksumProvider.h"
#include "FileMetadata.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main(int argc, char** argv) {
    hw8::Parameters params{argc, argv};
    // std::cout << params << std::endl;

    hw8::Md5ChecksumProvider csp;

    fs::path p1{"txt1.txt"};
    hw8::FileMetadata md1{p1, 3, csp};

    fs::path p2{"txt2.txt"};
    hw8::FileMetadata md2{p2, 3, csp};

    fs::path p3{"txt3.txt"};
    hw8::FileMetadata md3{p3, 3, csp};
    
    fs::path p4{"txt4.txt"};
    hw8::FileMetadata md4{p4, 3, csp};

    auto e1 = hw8::compare_equal(md1, md2);
    auto e2 = hw8::compare_equal(md1, md3);
    auto e3 = hw8::compare_equal(md1, md4);

    std::cout << "e1 = " << e1 << std::endl;
    std::cout << "e2 = " << e2 << std::endl;
    std::cout << "e3 = " << e3 << std::endl;

    return 0;
}
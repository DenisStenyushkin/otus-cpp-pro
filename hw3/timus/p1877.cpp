#include <iostream>
#include <string>


int main() {
    std::string s_code1, s_code2;
    std::getline(std::cin, s_code1);
    std::getline(std::cin, s_code2);

    int d_code1 = std::stoi(s_code1);
    int d_code2 = std::stoi(s_code2);

    if (d_code1 % 2 == 0 || d_code2 % 2 == 1) {
        std::cout << "yes" << std::endl;
    }
    else {
        std::cout << "no" << std::endl;
    }
}
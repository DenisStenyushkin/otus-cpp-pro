#include <iostream>


int main() {
    int n1, n2;
    std::cin >> n1 >> n2;

    int n_total = n1 + n2 - 1;
    
    std::cout << n_total - n1 << " " << n_total - n2 << std::endl;
    return 0;
}
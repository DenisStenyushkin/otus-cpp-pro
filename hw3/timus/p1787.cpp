#include <iostream>


int main() {
    int k, n, a, rem=0;

    std::cin >> k >> n;
    for(int i = 0; i < n; ++i) {
        std::cin >> a;
        rem = rem + a - k;
        rem = rem > 0 ? rem : 0;
    }

    std::cout << rem << std::endl;
}
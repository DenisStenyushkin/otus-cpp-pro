#include <iostream>
#include <functional>
#include <map>
#include <string>

#include "allocator.hpp"
#include "container.hpp"


template<class tkey, class tval, class comp, class alloc>
void print_map(std::string comment, const std::map<tkey, tval, comp, alloc>& m)
{
    std::cout << comment << ": ";

    for (const auto& [key, value] : m)
        std::cout << '[' << key << "] = " << value << "; ";

    std::cout << '\n';
}

template<class tval, class alloc>
void print_ll(std::string comment, hw3::LinkedListContainer<tval, alloc>& m)
{
    std::cout << comment << ": ";


    for (auto it = m.begin(); it != m.end(); ++it)
        std::cout << *it << "; ";

    std::cout << '\n';
}

int fac(int n) {
    if (n == 0) return 1;
    return n * fac(n-1);
}

int main() {
    std::map<int, int> map1;
    for (int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        map1.insert({i, fac(i)});
    }
    print_map("std::map default allocator", map1);

    std::map<int, int, std::less<int>, hw3::cpp_11_allocator<int, 10>> map2;
    for (int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        map2.insert({i, fac(i)});
    }
    print_map("std::map custom allocator", map2);

    hw3::LinkedListContainer<int> ll1;
    for (int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        ll1.push_back(i);
    }
    print_ll("hw3::LinkedListContainer default allocator", ll1);

    hw3::LinkedListContainer<int, hw3::cpp_11_allocator<int, 10>> ll2;
    for (int i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        ll2.push_back(i);
    }
    print_ll("hw3::LinkedListContainer custom allocator", ll2);
}
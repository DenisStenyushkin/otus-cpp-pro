#include <math.h>
#include <string>
#include <vector>


using Ip = std::vector<std::string>;

struct IpComparatorDesc {
    bool operator()(const Ip& lhs, const Ip& rhs) const {
        for(std::size_t i  = 0; i < lhs.size(); ++i) {
            auto l = std::stoi(lhs[i]);
            auto r = std::stoi(rhs[i]);
            if (l != r) {
                return l > r;
            }
        }
        return false;
    };
};

void print(const std::vector<Ip> ip_pool) {
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if (ip_part != ip->cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }
}

std::vector<Ip> filter(const std::vector<Ip> ip_pool, u_int8_t value) {
    std::vector<Ip> res;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(res),
        [value](Ip elem) { return std::stoi(elem[0]) == value; });
    return res;
}

std::vector<Ip> filter(const std::vector<Ip> ip_pool, u_int8_t value1, u_int8_t value2) {
    std::vector<Ip> res;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(res),
        [value1, value2](Ip elem) {
            return std::stoi(elem[0]) == value1 && std::stoi(elem[1]) == value2; 
        });
    return res;
}

std::vector<Ip> filter_any(const std::vector<Ip> ip_pool, u_int8_t value) {
    std::vector<Ip> res;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(res),
        [value](Ip elem) {
            return std::find(elem.begin(), elem.end(), std::to_string(value)) != elem.end();
        });
    return res;
}

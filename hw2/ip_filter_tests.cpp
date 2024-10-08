#include <string>
#include <vector>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "funcs.hpp"


TEST(IpComparatorDesc, OrdersOnFirstElement) {
    Ip ip1{"1", "2", "3", "4"};
    Ip ip2{"10", "2", "3", "4"};
    IpComparatorDesc comparator;

    ASSERT_EQ(comparator(ip1, ip2), false);
}

TEST(IpComparatorDesc, OrdersSecondElement) {
    Ip ip1{"1", "200", "3", "4"};
    Ip ip2{"1", "2", "3", "4"};
    IpComparatorDesc comparator;

    ASSERT_EQ(comparator(ip1, ip2), true);
}

TEST(IpComparatorDesc, FalseOnEqual) {
    Ip ip1{"1", "2", "3", "4"};
    Ip ip2{"1", "2", "3", "4"};
    IpComparatorDesc comparator;

    ASSERT_EQ(comparator(ip1, ip2), false);
}

TEST(IpComparatorDesc, LesserPosesNotAffect) {
    Ip ip1{"10", "2", "3", "4"};
    Ip ip2{"1", "20", "30", "40"};
    IpComparatorDesc comparator;

    ASSERT_EQ(comparator(ip1, ip2), true);
}

TEST(filter_1, FiltersOk) {
    std::vector<Ip> vec{
        {"1", "2", "3", "4"},
        {"10", "20", "30", "40"},
        {"1", "200", "201", "1"}
    };
    std::vector<Ip> exp{
        {"1", "2", "3", "4"},
        {"1", "200", "201", "1"}
    };

    std::vector<Ip> res = filter(vec, 1);

    ASSERT_THAT(res, testing::ElementsAreArray(exp));
}

TEST(filter_2, FiltersOk) {
    std::vector<Ip> vec{
        {"46", "70", "3", "4"},
        {"1", "70", "30", "40"},
        {"46", "200", "201", "1"},
        {"46", "200", "201", "1"},
        {"46", "200", "201", "70"}
    };
    std::vector<Ip> exp{
        {"46", "70", "3", "4"}
    };

    std::vector<Ip> res = filter(vec, 46, 70);

    ASSERT_THAT(res, testing::ElementsAreArray(exp));
}

TEST(filter_any, FiltersOk) {
    std::vector<Ip> vec{
        {"46", "70", "3", "4"},
        {"1", "70", "30", "40"},
        {"46", "200", "201", "1"},
        {"46", "200", "201", "1"},
        {"46", "200", "201", "70"}
    };
    std::vector<Ip> exp{
        {"46", "70", "3", "4"},
        {"46", "200", "201", "1"},
        {"46", "200", "201", "1"},
        {"46", "200", "201", "70"}
    };

    std::vector<Ip> res = filter_any(vec, 46);

    ASSERT_THAT(res, testing::ElementsAreArray(exp));
}

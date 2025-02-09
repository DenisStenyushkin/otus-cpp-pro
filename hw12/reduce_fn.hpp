#pragma once

#include "reducer.hpp"


Reducer::reduce_result_t count(Reducer::reduce_input_t::iterator begin,
                               Reducer::reduce_input_t::iterator end) {
    Reducer::reduce_result_t result;

    for(; begin != end; ++begin) {
        ++result[*begin];
    }

    return result;
}

#pragma once

#include "definitions/code.hpp"
#include <cstdint>

//
// For generic NB polar decoders
//
struct symbols_t
{
    int64_t value[_GF_];
    bool    is_freq;
};

struct symbols_t1
{
    bool  is_freq;
    float value[_GF_];
};

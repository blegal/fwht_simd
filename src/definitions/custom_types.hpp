#pragma once

#include <cstdint>
#include "definitions/code.hpp"

//
// For generic NB polar decoders
//
struct symbols_t {
    float value[_GF_];
    bool is_freq;
};

//
// For specialized NB polar decoders
//
struct symbols_s {
    float value[_GF_];
};


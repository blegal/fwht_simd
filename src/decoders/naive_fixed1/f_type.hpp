#pragma once
//
//
//
//
//
#include "include/ap_fixed.h"
//
#define NBITS 22
//
struct symbols_t
{
    ap_fixed<NBITS, 1> value[_GF_];
    // ap_fixed<NBITS, 1> value[_GF_];
    bool is_freq;
};

// struct symbols_t1
// {
//     ap_fixed<NBITS + 1, 2> value[_GF_];
//     // ap_fixed<NBITS, 1> value[_GF_];
//     bool is_freq;
// };

struct symbols_t2
{
    ap_fixed<NBITS + _logGF_, 1 + _logGF_> value[_GF_];
    // ap_fixed<NBITS, 1> value[_GF_];
    bool is_freq;
};
struct symbols_t3
{
    ap_fixed<2 * NBITS, 2> value[_GF_];
    // ap_fixed<NBITS, 1> value[_GF_];
    bool is_freq;
};
//
//
//
//
//
inline float is_issue(const ap_fixed<NBITS, 1> *value)
{
    float maxv = 0.f;
    for (size_t i = 0; i < _GF_; i++)
    {
        const float v = value[i].to_float();
        maxv          = std::fmaxf(maxv, std::fabs(v));
    }
    return maxv;
}
//
//
//
//
//

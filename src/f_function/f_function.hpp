#pragma once
//
//
//
//
//
#include "../structure.hpp"
#include "../fwht/fwht_x86.hpp"
#include "../const_config_GF64_N64.hpp"
//
//
//
//
//
template <uint32_t gf_size>
void f_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{

    if (src_a->is_freq == false)
    {
        fwht<gf_size>(src_a->value);
        src_a->is_freq = true;
    }

    if (src_b->is_freq == false)
    {
        fwht<gf_size>(src_b->value);
        src_b->is_freq = true;
    }

    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
        dst->gf   [i]  = src_a->gf  [i]; // to be removed !
    }
    dst->is_freq = true; // a.a we do CN in FD

    fwht<gf_size>(dst->value);
    dst->is_freq = false;

    value_type s1 = 0;
    for (size_t i = 0; i < gf_size; i++)
    {
        s1 += dst->value[i];
    }
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] /= s1;
    }
}
//
//
//
//
//

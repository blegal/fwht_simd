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
    //
    // Switch from time to frequency domain if needed
    //
    if (src_a->is_freq == false)
    {
        fwht<gf_size>(src_a->value);
        src_a->is_freq = true;
    }

    //
    // Switch from time to frequency domain if needed
    //  
    if (src_b->is_freq == false)
    {
        fwht<gf_size>(src_b->value);
        src_b->is_freq = true;
    }

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
        dst->gf   [i]  = src_a->gf  [i]; // to be removed !
    }
    dst->is_freq = true; // a.a we do CN in FD

    // BLG : we decide to not normalize the CN output in frequency domain
    //       normalization is done in only in VNs when moving to probability domain

//    fwht<gf_size>(dst->value);
//    dst->is_freq = false;

//    value_type s1 = 0;
//    for (size_t i = 0; i < gf_size; i++)
//    {
//        s1 += dst->value[i];
//    }
//    for (size_t i = 0; i < gf_size; i++)
//    {
//        dst->value[i] /= s1;
//    }
}
//
//
//
//
//

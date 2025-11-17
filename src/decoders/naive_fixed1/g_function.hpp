#pragma once

#include "f_argmax.hpp"
#include "f_normalize.hpp"

// #define _TEST_

// template <int gf_size>
// void g_function(
//     symbols_t * __restrict dst,   // the data to be computed for the left side of the graph
//     symbols_t * __restrict src_a, // the upper value set from the right side of the graph
//     symbols_t * __restrict src_b, // the lower value set from the right side of the graph
//     uint32_t    src_c)            // the computed symbols coming from the left side of the graph
// {
//     if (src_a->is_freq == true)
//     {
//         const float factor = norm_factor_lwht<gf_size>();
//         f_normalize<gf_size>(src_a->value, factor);
//         f_normalize<gf_size>(src_a->value, factor);
//         fwht       <gf_size>(src_a->value);
//         src_a->is_freq = false;
//     }

//     if (src_b->is_freq == true) {
//         const float factor = norm_factor_lwht<gf_size>();
//         f_normalize<gf_size>(src_b->value, factor);
//         f_normalize<gf_size>(src_b->value, factor);
//         fwht       <gf_size>(src_b->value);
//         src_b->is_freq = false;
//     }

// //  symbols_t tt;
//     for (size_t i = 0; i < gf_size; i++) {
//         const int idx = src_c ^ i;
//         dst->value[idx] = src_a->value[i] * src_b->value[idx];
// //      tt.value[i] = float(src_a->value[i]);
//     }
//     f_normalize<gf_size>(dst->value); // temporal
//     dst->is_freq = false;
// }
//
//
//
//
//

template <int gf_size>
void g_function(
    symbols_t *__restrict dst,   // the data to be computed for the left side of the graph
    symbols_t *__restrict src_a, // the upper value set from the right side of the graph
    symbols_t *__restrict src_b, // the lower value set from the right side of the graph
    uint32_t src_c)              // the computed symbols coming from the left side of the graph
{
    if (src_a->is_freq == true)
    {
        // const float factor = norm_factor_lwht<gf_size>();
        // f_normalize<gf_size>(src_a->value, factor);
        // f_normalize<gf_size>(src_a->value, factor);
        // fwht       <gf_size>(src_a->value);
        // src_a->is_freq = false;

        symbols_t2 src_a1;
        for (int i = 0; i < gf_size; i++)
        {
            src_a1.value[i] = (ap_fixed<NBITS + _logGF_, 1 + _logGF_>)src_a->value[i];
        }

        fwht<gf_size>(src_a1.value);
        LZC_shift_after_fwht<gf_size>(src_a1.value, src_a->value);
        src_a->is_freq = false;
    }

    if (src_b->is_freq == true)
    {
        // const float factor = norm_factor_lwht<gf_size>();
        // f_normalize<gf_size>(src_b->value, factor);
        // f_normalize<gf_size>(src_b->value, factor);
        // fwht       <gf_size>(src_b->value);
        // src_b->is_freq = false;
        symbols_t2 src_b1;
        for (int i = 0; i < gf_size; i++)
        {
            src_b1.value[i] = (ap_fixed<NBITS + _logGF_, 1 + _logGF_>)src_b->value[i];
        }
        src_b1.is_freq = src_b->is_freq;

        fwht<gf_size>(src_b1.value);
        LZC_shift_after_fwht<gf_size>(src_b1.value, src_b->value);
    }

    // for (size_t i = 0; i < gf_size; i++)
    // {
    //     const int idx   = src_c ^ i;
    //     dst->value[idx] = src_a->value[i] * src_b->value[idx];
    // }
    // f_normalize<gf_size>(dst->value); // temporal
    // dst->is_freq = false;

    symbols_t3 dst2;

    for (size_t i = 0; i < gf_size; i++)
    {
        const int idx   = src_c ^ i;
        dst2.value[idx] = src_a->value[i] * src_b->value[idx];
    }
    dst->is_freq = false;
    LZC_shift_after_fwht_mult<gf_size>(dst2.value, dst->value);
    // std::cout << "\033c" << std::flush;
    // for (int i = 0; i < gf_size; i++)
    // {
    //     printf("g_func %d : %.20f | %.20f\n", i, (float)dst2.value[i], (float)dst->value[i]);
    // }
}
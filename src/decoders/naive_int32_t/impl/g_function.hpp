#pragma once

#include "../arch/i_argmax.hpp"
#include "../arch/i_fwht.hpp"
#include "../arch/i_normalize.hpp"

template <int gf_size>
void g_function(
    symbols_i<gf_size> * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_i<gf_size> * __restrict src_a, // the upper value set from the right side of the graph
    symbols_i<gf_size> * __restrict src_b, // the lower value set from the right side of the graph
    uint32_t    src_c)            // the computed symbols coming from the left side of the graph
{
    if (src_a->is_freq == true)
    {
//      const float factor = norm_factor_lwht<gf_size>();
//      i_normalize<gf_size>(src_a->value, factor);
//      i_normalize<gf_size>(src_a->value, factor);
        //
        // two consecutives NORM or SCALING seems to be
        // equals to DIVISION by GF
        //
        for (int i = 0; i < gf_size; i++)
            src_a->value[i] /= gf_size;
        fwht<gf_size>(src_a->value);
        src_a->is_freq = false;
    }

    if (src_b->is_freq == true) {
//      const float factor = norm_factor_lwht<gf_size>();
//      i_normalize<gf_size>(src_b->value, factor);
//      i_normalize<gf_size>(src_b->value, factor);
        //
        // two consecutives NORM or SCALING seems to be
        // equals to DIVISION by GF
        //
        for (int i = 0; i < gf_size; i++)
            src_b->value[i] /= gf_size;
        fwht       <gf_size>(src_b->value);
        src_b->is_freq = false;
    }

    for (size_t i = 0; i < gf_size; i++) {
        const int idx = src_c ^ i;
        const int64_t a = src_a->value[i];
        const int64_t b = src_b->value[idx];
        const int64_t c = a * b;
        dst->value[idx] = (int32_t)(c >> 32);
    }
    i_normalize<gf_size>(dst->value); // temporal
    dst->is_freq = false;
}
//
//
//
//
//

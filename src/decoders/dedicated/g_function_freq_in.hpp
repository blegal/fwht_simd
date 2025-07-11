#pragma once
//
//
//
//
//
#include "features/archi.hpp"
#include "definitions/custom_types.hpp"
//
//
//
//
//
template <int gf_size> inline __attribute__((always_inline)) void g_function_freq_in(
    symbols_t * dst,   // the data to be computed for the left side of the graph
    symbols_t * src_a, // the upper value set from the right side of the graph
    symbols_t * src_b, // the lower value set from the right side of the graph
    const uint16_t* src_c,    // the computed symbols coming from the left side of the graph
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        FWHT_NORM<gf_size>(src_a[s].value);
        src_a[s].is_freq = false;

        FWHT_NORM<gf_size>(src_b[s].value);
        src_b[s].is_freq = false;

        for (int i = 0; i < gf_size; i++) {
            const int   idx = src_c[s] ^ i;
            const float val = src_a[s].value[i] * src_b[s].value[idx];
            dst[s].value[idx] = val;
        }
        normalize<gf_size>(dst[s].value);
        dst[s].is_freq = false;
    }
}
//
//
//
//
//

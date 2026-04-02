#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

template <int gf_size>
void naive_scl_genie<gf_size>::f_function(symbols_t * __restrict dst, symbols_t * __restrict src_a, symbols_t * __restrict src_b) {
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        FWHT<gf_size>(src_a->value);
        src_a->is_freq = true;
    }

    if (src_b->is_freq == false) // Switch from time to frequency domain
    {
        FWHT<gf_size>(src_b->value);
        src_b->is_freq = true;
    }

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++) {
        dst->value[i] = src_a->value[i] * src_b->value[i];
    }
    dst->is_freq = true; // a.a we do CN in FD
}
//
//
//
//
//
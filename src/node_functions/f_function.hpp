#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(symbols_t * dst, symbols_t * src_a, symbols_t * src_b) {
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        FWHT_NORM<gf_size>(src_a->value);
        src_a->is_freq = true;
    }

    if (src_b->is_freq == false) // Switch from time to frequency domain
    {
        FWHT_NORM<gf_size>(src_b->value);
        src_b->is_freq = true;
    }

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++) {
        dst->value[i] = 10.f * src_a->value[i] * src_b->value[i]; // TODO : attention au facteur 10x qui est magique !!!
    }
    dst->is_freq = true; // a.a we do CN in FD
}
//
//
//
//
//

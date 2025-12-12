#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(symbols_t * __restrict dst, symbols_t * __restrict src_a, symbols_t * __restrict src_b)
{
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
    for (size_t i = 0; i < gf_size; i++)
    {
#if 0
        dst->value[i] = 10.f * src_a->value[i] * src_b->value[i];
#else
        dst->value[i] = src_a->value[i] * src_b->value[i];
#endif
    }
    dst->is_freq = true; // a.a we do CN in FD
}
//
//
//
//
//
template <uint32_t gf_size>
void f_function_proba_only(symbols_t * __restrict dst, symbols_t * __restrict src_a, symbols_t * __restrict src_b)
{
    symbols_t tmp_a = *src_a;
    symbols_t tmp_b = *src_b;

    FWHT_NORM<gf_size>(tmp_a.value);
    FWHT_NORM<gf_size>(tmp_b.value);

    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = tmp_a.value[i] * tmp_b.value[i];
    }

    FWHT_NORM<gf_size>(dst->value);
    normalize<gf_size>(dst->value);
    dst->is_freq = false;
}
//
//
//
//
//

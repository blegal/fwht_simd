#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(symbols_f * __restrict dst, symbols_f * __restrict src_a, symbols_f * __restrict src_b)
{
#ifdef _TEST_
    printf("(DD) f_function(--)\n");
#endif

    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        fwht_norm<gf_size>(src_a->value);
        src_a->is_freq = true;
    }
#ifdef _TEST_
    if ( is_issue(src_a->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        printf("(EE) value = %f\n", is_issue(src_a->value));
        exit(EXIT_FAILURE);
    }
#endif

    if (src_b->is_freq == false) // Switch from time to frequency domain
    {
        fwht_norm<gf_size>(src_b->value);
        src_b->is_freq = true;
    }

#ifdef _TEST_
    if ( is_issue(src_b->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        printf("(EE) value = %f\n", is_issue(src_b->value));
        exit(EXIT_FAILURE);
    }
#endif

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
#if 0
    const ap_fixed<NBITS, NFRAC> fact = 8.f; // TODO : attention au facteur 10x qui est magique !!!
    for (size_t i = 0; i < gf_size; i++) {
        dst->value[i] =  fact * src_a->value[i] * src_b->value[i];
    }
#else
    for (size_t i = 0; i < gf_size; i++) {
        dst->value[i] =  src_a->value[i] * src_b->value[i];
    }
    const float fact = 1.f / dst->value[0].to_float();
    const ap_fixed<16, 16> ffact = fact;    // should be drastically reduced !
    for (size_t i = 0; i < gf_size; i++) {
        dst->value[i] = ffact * dst->value[i];
    }
#endif
    dst->is_freq = true; // a.a we do CN in FD

#ifdef _TEST_
    if ( is_issue(dst->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        printf("(EE) value = %f\n", is_issue(dst->value));
        exit(EXIT_FAILURE);
    }
#endif

}
//
//
//
//
//

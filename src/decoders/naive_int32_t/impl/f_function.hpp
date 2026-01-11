#pragma once

#include "../arch/i_fwht.hpp"
#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(
    symbols_i<gf_size> * __restrict dst,
    symbols_i<gf_size> * __restrict src_a,
    symbols_i<gf_size> * __restrict src_b) {
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        fwht<gf_size>(src_a->value);
#if FWHT_COUNTER_ENABLE
        fwht_call_counter += 1;
#endif
        src_a->is_freq = true;
    }
    if (src_b->is_freq == false) {
        fwht<gf_size>(src_b->value);
#if FWHT_COUNTER_ENABLE
        fwht_call_counter += 1;
#endif
        src_b->is_freq = true;
    }
    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++) {
        const int64_t a = src_a->value[i];
        const int64_t b = src_b->value[i];
        const int64_t c = a * b;
        dst->value[i]   = (int32_t) (c >> 28);
//        printf("F %3zu : %lld * %lld = %d\n", i, a, b, dst->value[i]);
    }
#if 0
    printf("F function (result)"); show<gf_size>(dst->value);
#endif
    dst->is_freq = true;
}
//
//
//
//
//

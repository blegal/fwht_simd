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
#if NEW_QUANTIF == 0
    for (size_t i = 0; i < gf_size; i++) {
        const int64_t a = src_a->value[i];
        const int64_t b = src_b->value[i];
        const int64_t c = a * b;
        dst->value[i]   = (int32_t) (c >> symbols_i_shift);
    }
#else
    int64_t tmp[gf_size];
    for (size_t i = 0; i < gf_size; i++) {
        const int64_t a = src_a->value[i];
        const int64_t b = src_b->value[i];
        tmp[i]          = a * b;
    }
    int64_t maxv = 0;
    for (int i = 0; i < gf_size; i++) {
        const int64_t a = tmp[i];
        const int64_t b = (a < 0) ? -a : a;
        maxv = (maxv > b) ? maxv : b;
    }
#ifdef _DEBUG_Q_
    printf("F function (result) with maxv = %lld\n", maxv);
    if ( maxv == 0 ) {
        printf(" - A values\n");
        show<gf_size>(src_a->value);
        printf(" - B values\n");
        show<gf_size>(src_b->value);
    }
    show<gf_size>(tmp);
#endif
    if ( maxv > symbols_i_iscale ) {
        const int64_t factor = (maxv / symbols_i_iscale) + 1;
        for (size_t i = 0; i < gf_size; i++)
            dst->value[i]   = (int32_t) (tmp[i] / factor );
        for (size_t i = 0; i < gf_size; i++)
            dst->value[i]   = dst->value[i] == 0 ? 1 : dst->value[i];
    }else {
        const int64_t factor = (symbols_i_iscale / maxv) + 1;
        for (size_t i = 0; i < gf_size; i++)
            dst->value[i]   = (int32_t) (tmp[i] * factor );
        for (size_t i = 0; i < gf_size; i++)
            dst->value[i]   = dst->value[i] == 0 ? 1 : dst->value[i];
    }
#ifdef _DEBUG_Q_
    show<gf_size>(dst->value);
#endif
#endif
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

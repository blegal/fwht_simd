#pragma once

#include "../arch/i_argmax.hpp"
#include "../arch/i_fwht.hpp"
#include "../arch/i_normalize.hpp"
#include "definitions/custom_types.hpp"

template <int gf_size>
void g_function(
    symbols_i<gf_size> * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_i<gf_size> * __restrict src_a, // the upper value set from the right side of the graph
    symbols_i<gf_size> * __restrict src_b, // the lower value set from the right side of the graph
    uint32_t src_c)                        // the computed symbols coming from the left side of the graph
{
    if (src_a->is_freq == true) {
#if NEW_QUANTIF == 0
        for (int i = 0; i < gf_size; i++)
            src_a->value[i] /= gf_size;
#endif
        fwht<gf_size>(src_a->value);
        src_a->is_freq = false;
    }

    if (src_b->is_freq == true) {
#if NEW_QUANTIF == 0
        for (int i = 0; i < gf_size; i++)
            src_b->value[i] /= gf_size;
#endif
        fwht<gf_size>(src_b->value);
        src_b->is_freq = false;
    }
#if NEW_QUANTIF == 0
    for (size_t i = 0; i < gf_size; i++) {
        const int     idx = src_c ^ i;
        const int64_t a   = src_a->value[i];
        const int64_t b   = src_b->value[idx];
        const int64_t c   = a * b;
        dst->value[idx]   = (int32_t) (c >> symbols_i_shift);
    }
#else
    int64_t tmp[gf_size];
    for (size_t i = 0; i < gf_size; i++) {
        const int     idx = src_c ^ i;
        const int64_t a   = src_a->value[i];
        const int64_t b   = src_b->value[idx];
        tmp[idx]          = a * b;
    }
    int64_t maxv = 0;
    for (size_t i = 0; i < gf_size; i++) {
        const int64_t a = tmp[i];
        const int64_t b = (a < 0) ? -a : a;
        maxv += b;
    }
    printf("G function (result) with maxv = %lld\n", maxv);
    show<gf_size>(tmp);
    if ( maxv > symbols_i_iscale ) {
        const int64_t factor = maxv / symbols_i_iscale;
        for (size_t i = 0; i < gf_size; i++)
            dst->value[i]   = (int32_t) (tmp[i] / factor  );
    }else {
        const int64_t factor = symbols_i_iscale / maxv;
        for (size_t i = 0; i < gf_size; i++)
            dst->value[i]   = (int32_t) (tmp[i] * factor);
    }
    show<gf_size>(dst->value);
#endif
//    show<gf_size>(dst->value);
    i_normalize<gf_size>(dst->value); // temporal
//    show<gf_size>(dst->value);
//    exit( EXIT_FAILURE );
    dst->is_freq = false;
}
//
//
//
//
//

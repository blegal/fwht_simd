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
//        show<gf_size>( src_a->value );
        for (int i = 0; i < gf_size; i++)
            src_a->value[i] /= gf_size;
//        show<gf_size>( src_a->value );
        fwht<gf_size>(src_a->value);
//        show<gf_size>( src_a->value );
        src_a->is_freq = false;
    }

    if (src_b->is_freq == true) {
        //for (int i = 0; i < gf_size; i++)
        //    printf("G- %3d : %d\n", i, src_b->value[i]);
        for (int i = 0; i < gf_size; i++)
            src_b->value[i] /= gf_size;
        //for (int i = 0; i < gf_size; i++)
        //    printf("GB %3d : %d\n", i, src_b->value[i]);
        fwht<gf_size>(src_b->value);
        //for (int i = 0; i < gf_size; i++)
        //    printf("GC %3d : %d\n", i, src_b->value[i]);
        src_b->is_freq = false;
    }

    for (size_t i = 0; i < gf_size; i++) {
        const int     idx = src_c ^ i;
        const int64_t a   = src_a->value[i];
        const int64_t b   = src_b->value[idx];
        const int64_t c   = a * b;
        dst->value[idx]   = (int32_t) (c >> 28);
        //printf("G %3zu : %lld * %lld = %d\n", i, a, b, dst->value[i]);
    }

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

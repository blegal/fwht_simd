#pragma once

#include "f_argmax.hpp"
#include "features/archi.hpp"

template <int gf_size>
void g_function(
    symbols_i64<gf_size> * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_i64<gf_size> * __restrict src_a, // the upper value set from the right side of the graph
    symbols_i64<gf_size> * __restrict src_b, // the lower value set from the right side of the graph
    uint32_t src_c)                          // the computed symbols coming from the left side of the graph
{
    if (src_a->is_freq == true) {

        int64_t temp[gf_size];
        for (int i = 0; i < gf_size; i++) {
            temp[i] = src_a->value[i];
        }

        I64_FWHT<gf_size>(temp);
        LZC_normalize<gf_size, INTEGER_BITS>(temp);
        for (int i = 0; i < gf_size; i++) {
            src_a->value[i] = temp[i];
        }
        src_a->is_freq = false;
    }

    if (src_b->is_freq == true) {

        int64_t temp[gf_size];
        for (int i = 0; i < gf_size; i++) {
            temp[i] = src_b->value[i];
        }

        I64_FWHT<gf_size>(temp);
        LZC_normalize<gf_size, INTEGER_BITS>(temp);
        for (int i = 0; i < gf_size; i++) {
            src_b->value[i] = temp[i];
        }
        src_b->is_freq = false;
    }

            for(int i = 0; i < gf_size; i++) {
            if (src_a->value[i] <= 0) {
                src_a->value[i] = 1;
            }
            if (src_b->value[i] <= 0) {
                src_b->value[i] = 1;
            }
        }

    int64_t temp_dst[gf_size];
    for (int i = 0; i < gf_size; i++) {
        temp_dst[i] = 0;
    }
    for (size_t i = 0; i < gf_size; i++) {
        const int idx = src_c ^ i;
        temp_dst[idx] = static_cast<int64_t>(src_a->value[i]) * static_cast<int64_t>(src_b->value[idx]);
    }
    dst->is_freq = false;
    LZC_normalize<gf_size, INTEGER_BITS>(temp_dst);
    for (int i = 0; i < gf_size; i++) {
        dst->value[i] = temp_dst[i];
    }
}
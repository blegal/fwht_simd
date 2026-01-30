#pragma once

#include "definitions/custom_types.hpp"
// #include "f_fwht.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(symbols_i32<gf_size> * __restrict dst, symbols_i32<gf_size> * __restrict src_a, symbols_i32<gf_size> * __restrict src_b) {
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        int32_t temp[gf_size];
        for (size_t i = 0; i < gf_size; i++) {
            temp[i] = src_a->value[i];
        }
        I32_FWHT<gf_size>(temp);
        LZC_normalize<gf_size, NBITS>(temp);
        for (size_t i = 0; i < gf_size; i++) {
            src_a->value[i] = temp[i];
        }
        src_a->is_freq = true;
    }
    if (src_b->is_freq == false) {
        int32_t temp[gf_size];
        for (size_t i = 0; i < gf_size; i++) {
            temp[i] = src_b->value[i];
        }
        I32_FWHT<gf_size>(temp);
        LZC_normalize<gf_size, NBITS>(temp);
        for (size_t i = 0; i < gf_size; i++) {
            src_b->value[i] = temp[i];
        }
        src_b->is_freq = true;
    }
    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    int32_t temp_dst[gf_size];
    for (size_t i = 0; i < gf_size; i++) {
        temp_dst[i] = (src_a->value[i]) * (src_b->value[i]);
    }
    dst->is_freq = true;
    LZC_normalize<gf_size, NBITS>(temp_dst);
    for (size_t i = 0; i < gf_size; i++) {
        dst->value[i] = temp_dst[i];
    }

    // std::cout << "\033c" << std::flush;
    // for (int i = 0; i < gf_size; i++)
    // {
    //     printf("f_func %d : %.20f | %.20f\n", i, (float)dst2.value[i], (float)dst->value[i]);
    // }
}
//
//
//
//
//

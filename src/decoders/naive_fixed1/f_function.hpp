#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(symbols_f *__restrict dst, symbols_f *__restrict src_a, symbols_f *__restrict src_b)
{
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        symbols_f2 src_a1;
        for (int i = 0; i < gf_size; i++)
        {
            src_a1.value[i] = (ap_fixed<NBITS + _logGF_, 1 + _logGF_>)src_a->value[i];
        }

        // std::cout << "\033c" << std::flush;
        // for (int i = 0; i < gf_size; i++)
        // {
        //     printf("fwht_in_prob %d: %.20f | %.20f\n", i, (float)src_a->value[i], (float)src_a1.value[i]);
        // }
        fwht<gf_size>(src_a1.value);
        LZC_shift_after_fwht<gf_size>(src_a1.value, src_a->value);

        // for (int i = 0; i < gf_size; i++)
        // {
        //     printf("fwht %d: %.20f | %.20f\n", i, (float)src_a->value[i], (float)src_a1.value[i]);
        // }
        src_a->is_freq = true;
    }
    if (src_b->is_freq == false)
    {
        symbols_f2 src_b1;
        for (int i = 0; i < gf_size; i++)
        {
            src_b1.value[i] = (ap_fixed<NBITS + _logGF_, 1 + _logGF_>)src_b->value[i];
        }

        fwht<gf_size>(src_b1.value);
        LZC_shift_after_fwht<gf_size>(src_b1.value, src_b->value);
        // std::cout << "\033c" << std::flush;

        // for (int i = 0; i < gf_size; i++)
        // {
        //     printf("fwht %d: %.20f | %.20f\n", i, (float)src_b->value[i], (float)src_b1.value[i]);
        // }
        src_b->is_freq = true;
    }
    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    symbols_f3 dst2;

    for (size_t i = 0; i < gf_size; i++)
    {
        dst2.value[i] = src_a->value[i] * src_b->value[i];
    }
    dst->is_freq = true;
    LZC_shift_after_fwht_mult<gf_size>(dst2.value, dst->value);

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

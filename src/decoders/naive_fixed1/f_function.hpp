#pragma once

#include "definitions/custom_types.hpp"
#include "f_fwht.hpp"
#include "features/archi.hpp"

template <uint32_t gf_size>
void f_function(symbols_t *__restrict dst, symbols_t *__restrict src_a, symbols_t *__restrict src_b)
{
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {

        // std::cout << "\033c" << std::flush;
        // for (int i = 0; i < gf_size; i++)
        // {
        //     printf("fwht_in_prob %d: %.20f | %.20f\n", i, (float)src_a->value[i], (float)src_a1.value[i]);
        // }
        // std::cout << "\033c" << std::flush;

        // for (int i = 0; i < gf_size; i++)
        // {
        //     printf("before fwht %d: %.30d\n", i, src_a->value[i]);
        // }

        fwht<gf_size>(src_a->value);
        LZC_normalize<gf_size>(src_a->value);

        // std::cout << "\033c" << std::flush;
        // for (int i = 0; i < gf_size; i++)
        // {
        //     printf("fwht %d: %.30d\n", i, src_a->value[i]);
        // }
        src_a->is_freq = true;
    }
    if (src_b->is_freq == false)
    {

        fwht<gf_size>(src_b->value);
        LZC_normalize<gf_size>(src_b->value);
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
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
    }
    dst->is_freq = true;
    LZC_normalize<gf_size>(dst->value);

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

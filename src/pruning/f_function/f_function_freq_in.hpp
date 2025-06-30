#pragma once
//
//
//
//
//
#include "../../structure.hpp"
#include "../../fwht/fwht_x86.hpp"
#include "../../const_config_GF64_N64.hpp"
//
//
//
//
//
//#define debug_f_function
template <uint32_t gf_size>
void f_function_freq_in(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{
#if _AUTO_CHECK_
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }

    if (src_b->is_freq == false) // Switch from time to frequency domain
    {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }
#endif
    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
//        dst->gf   [i] = src_a->gf   [i]; // to be removed !
    }
    dst->is_freq = true;
}
//
//
//
//
//

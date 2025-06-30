#pragma once
//
//
//
//
//
#include "../structure.hpp"
#include "../fwht/fwht_x86.hpp"
#include "../const_config_GF64_N64.hpp"
//
//
//
//
//
//#define debug_f_function
template <uint32_t gf_size>
void f_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{
    if (src_a->is_freq == false) // Switch from time to frequency domain
    {
#if defined(debug_f_function)
        printf("[debug f] > Conversion Proba to Freq. (src_a) [%d]\n", idx);
#endif
        fwht<gf_size>(src_a->value);
        normalize<gf_size>(src_a->value, 0.125);
        src_a->is_freq = true;
    }

    if (src_b->is_freq == false) // Switch from time to frequency domain
    {
#if defined(debug_f_function)
        printf("[debug f] > Conversion Proba to Freq. (src_b) [%d]\n", idx);
#endif
        fwht<gf_size>(src_b->value);
        normalize<gf_size>(src_b->value, 0.125);
        src_b->is_freq = true;
    }

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
//        dst->gf   [i] = src_a->gf   [i]; // to be removed !
    }
    dst->is_freq = true; // a.a we do CN in FD

#if 0        
    if( result_in_proba )
    {
        fwht<gf_size>(dst->value);
        normalize<gf_size>(dst->value, 0.125);
        dst->is_freq = false;
        normalize<gf_size>(dst->value);
        // TODO
        printf("[debug f] < Conversion Freq. to Proba (dst) [%d]\n", idx);
    }
#endif
//#if defined(debug_f_function)
//    printf("[debug f]"); show_symbols( dst );
//#endif
}
//
//
//
//
//

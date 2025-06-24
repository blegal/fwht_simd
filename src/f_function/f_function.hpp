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
template <uint32_t gf_size>
void f_function(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{
    //
    // Switch from time to frequency domain if needed
    //
//  printf("Proba(src_a) : "); show_symbols( src_a );
    if (src_a->is_freq == false)
    {
        fwht<gf_size>(src_a->value);
        src_a->is_freq = true;
    }
//  printf("Frequ(src_a) : "); show_symbols( src_a );

    //
    // Switch from time to frequency domain if needed
    //  
//  printf("Proba(src_b) : "); show_symbols( src_b );
    if (src_b->is_freq == false)
    {
        fwht<gf_size>(src_b->value);
        src_b->is_freq = true;
    }
//  printf("Frequ(src_b) : "); show_symbols( src_b );

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
        dst->gf   [i]  = src_a->gf  [i]; // to be removed !
    }
    dst->is_freq = true; // a.a we do CN in FD

    fwht<gf_size>(dst->value);
    dst->is_freq = false;
    normalize<gf_size>(dst->value); // Normalize the output in frequency domain

//  printf("Frequ(dst) : "); show_symbols( dst );
//  exit( EXIT_FAILURE ); // BLG : we stop here to check the output of the CN

    // BLG : we decide to not normalize the CN output in frequency domain
    //       normalization is done in only in VNs when moving to probability domain

//    fwht<gf_size>(dst->value);
//    dst->is_freq = false;

//    value_type s1 = 0;
//    for (size_t i = 0; i < gf_size; i++)
//    {
//        s1 += dst->value[i];
//    }
//    for (size_t i = 0; i < gf_size; i++)
//    {
//        dst->value[i] /= s1;
//    }
}
//
//
//
//
//

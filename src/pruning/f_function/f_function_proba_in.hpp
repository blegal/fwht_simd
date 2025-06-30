#pragma once
//
//
//
//
//
#include "../../structure.hpp"
#include "../../fwht/fwht_x86.hpp"
#include "../../fwht/fwht_neon.hpp"
#include "../../fwht/fwht_norm_neon.hpp"
#include "../../fwht/fwht_avx2.hpp"
#include "../../fwht/fwht_norm_avx2.hpp"
#include "../../const_config_GF64_N64.hpp"
//
//
//
//
//
//#define debug_f_function
template <uint32_t gf_size>
void f_function_proba_in(symbols_t *dst, symbols_t *src_a, symbols_t *src_b)
{
#if _AUTO_CHECK_
    if (src_a->is_freq == true) // Switch from time to frequency domain
    {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }

    if (src_b->is_freq == true)
    {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }
#endif
    symbols_t tmp_a;
    for (int i = 0; i < gf_size; i++)
        tmp_a.value[i] = src_a->value[i];
#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>(tmp_a.value);
//#elif defined(__AVX2__)
//    fwht_norm_avx2<gf_size>(tmp_a.value);
#else
    fwht<gf_size>(tmp_a.value);
    normalize<gf_size>(tmp_a.value, 0.125);
#endif

    tmp_a.is_freq = true;

    symbols_t tmp_b;
    for (int i = 0; i < gf_size; i++)
        tmp_b.value[i] = src_b->value[i];
#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>(tmp_b.value);
//#elif defined(__AVX2__)
//    fwht_norm_avx2<gf_size>(tmp_b.value);
#else
    fwht<gf_size>(tmp_b.value);
    normalize<gf_size>(tmp_b.value, 0.125);
#endif
    tmp_b.is_freq = true;

    //
    // Element-wise multiplication of the two input symbols because we are in frequency domain !
    //
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = tmp_a.value[i] * tmp_b.value[i];
//        dst->gf   [i] = src_a->gf  [i]; // to be removed !
    }
    dst->is_freq = true; // a.a we do CN in FD
//    show_symbols( dst );
//    exit( 0 );
}
//
//
//
//
//

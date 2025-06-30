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

#include "../../hadamard/hadamard_64.hpp"
#include "../../const_config_GF64_N64.hpp"
//
//
//
//
//
//#define debug_g_function
template <int gf_size>
void g_function_freq_in(
    symbols_t* dst,     // the data to be computed for the left side of the graph
    symbols_t* src_a,   // the upper value set from the right side of the graph
    symbols_t* src_b,   // the lower value set from the right side of the graph
    uint32_t   src_c)   // the computed symbols coming from the left side of the graph
{   
    //
    //  Process the mulitplication to support precomputed symbols
    //
#if _AUTO_CHECK_
    if( src_a->is_freq == false )
    {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }
    if( src_b->is_freq == false ) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }
#endif
#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>     (src_a->value       );
//#elif defined(__AVX2__)
//    fwht_norm_avx2<gf_size>(src_a->value);
#else
    fwht<gf_size>     (src_a->value       );
    normalize<gf_size>(src_a->value, 0.125);
#endif
    src_a->is_freq = false;

#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>     (src_b->value       );
//#elif defined(__AVX2__)
//    fwht_norm_avx2<gf_size>(src_b->value);
#else
    fwht<gf_size>     (src_b->value       );
    normalize<gf_size>(src_b->value, 0.125);
#endif
    src_b->is_freq = false;

    // Abdallah computations ...
    for (size_t i = 0; i < gf_size; i++)
    {
        const int idx = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
//        dst->gf   [i  ] = idx;//src_a->gf   [i]; // to be removed !
    }

    normalize<gf_size>( dst->value ); // temporal
    dst->is_freq = false;
}
//
//
//
//
//

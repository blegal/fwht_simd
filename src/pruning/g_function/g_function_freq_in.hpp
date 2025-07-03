#pragma once
//
//
//
//
//
#include "utilities/utility_functions.hpp"
#include "fwht/fwht.hpp"
#include "fwht/fwht_avx2.hpp"
#include "fwht/fwht_neon.hpp"
#include "fwht/fwht_norm_avx2.hpp"
#include "fwht/fwht_norm_neon.hpp"

#include "definitions/const_config_GF64_N64.hpp"
#include "../../hadamard/hadamard_64.hpp"
//
//
//
//
//
// #define debug_g_function
template <int gf_size> inline __attribute__((always_inline))
void g_function_freq_in(
    symbols_t * dst,   // the data to be computed for the left side of the graph
    symbols_t * src_a, // the upper value set from the right side of the graph
    symbols_t * src_b, // the lower value set from the right side of the graph
    const uint32_t    src_c)    // the computed symbols coming from the left side of the graph
{
#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>(src_a->value);
#elif defined(__AVX2__)
    fwht_norm_avx2<gf_size>(src_a->value);
//    fwht_avx2<gf_size>(src_a->value);
//    normalize<gf_size>(src_a->value, 0.125);
#else
    fwht<gf_size>(src_a->value);
    normalize<gf_size>(src_a->value, 0.125);
#endif
    src_a->is_freq = false;

#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>(src_b->value);
#elif defined(__AVX2__)
    fwht_norm_avx2<gf_size>(src_b->value);
//    fwht_avx2<gf_size>(src_b->value);
//    normalize<gf_size>(src_b->value, 0.125);
#else
    fwht<gf_size>(src_b->value);
    normalize<gf_size>(src_b->value, 0.125);
#endif
    src_b->is_freq = false;

    // Abdallah computations ...
    for (size_t i = 0; i < gf_size; i++) {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
    }

    normalize<gf_size>(dst->value); // temporal
    dst->is_freq = false;
}
//
//
//
//
//

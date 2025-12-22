#pragma once
//
//
//
//
//
#include "definitions/custom_types.hpp"
#include "features/archi.hpp"
//
//
//
//
//
#include "hadamard/Hadamard.hpp"
//
//
//
//
//
template <int gf_size>
inline __attribute__((always_inline)) void g_function_freq_in(
    symbols_s<gf_size> * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_s<gf_size> * __restrict src_a, // the upper value set from the right side of the graph
    symbols_s<gf_size> * __restrict src_b, // the lower value set from the right side of the graph
    const uint32_t src_c)         // the computed symbols coming from the left side of the graph
{
    const float* H = get_Hadamard_line<gf_size>(src_c);
    for (size_t i = 0; i < gf_size; i++)
        dst->value[i] = src_a->value[i] * H[i];

    FWHT<gf_size>(dst->value);
    FWHT<gf_size>(src_b->value);

    for (size_t i = 0; i < gf_size; i++)
        dst->value[i] = dst->value[i] * src_b->value[i];

    normalize<gf_size>(dst->value);
}
//
//
//
//
//
template <int gf_size> inline __attribute__((always_inline))
void g_function_proba_in(
          symbols_s<gf_size> * __restrict dst,   // the data to be computed for the left side of the graph
    const symbols_s<gf_size> * __restrict src_a, // the upper value set from the right side of the graph
    const symbols_s<gf_size> * __restrict src_b, // the lower value set from the right side of the graph
    const uint32_t    src_c  // the computed symbols coming from the left side of the graph
) {
#if 0
    for (int i = 0; i < gf_size; i += 1)
    {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i];
    }
    for (int i = 0; i < gf_size; i += 1)
    {
        dst->value[i] = dst->value[i] * src_b->value[i];
    }
#else
    for (int i = 0; i < gf_size; i += 1)
    {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
    }
#endif
    normalize<gf_size>(dst->value); // temporal
}
//
//
//
//
//

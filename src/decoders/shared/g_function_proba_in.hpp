#pragma once

#include "utilities/utility_functions.hpp"
#include "features/archi.hpp"

// #define debug_g_function
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

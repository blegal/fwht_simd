#pragma once

#include "utilities/utility_functions.hpp"
#include "fwht/fwht.hpp"

// #define debug_g_function
template <int gf_size> inline __attribute__((always_inline))
void g_function_proba_in(
          symbols_t * dst,   // the data to be computed for the left side of the graph
    const symbols_t * src_a, // the upper value set from the right side of the graph
    const symbols_t * src_b, // the lower value set from the right side of the graph
    const uint32_t    src_c  // the computed symbols coming from the left side of the graph
) {
    for (int i = 0; i < int(gf_size); i++) {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
    }
    normalize<gf_size>(dst->value); // temporal
    dst->is_freq = false;
}

#pragma once
//
//
//
//
//
#include <ios>

#include "features/archi.hpp"
#include "definitions/custom_types.hpp"
#include "utilities/utility_functions.hpp"
//
//
//
//
//
template <int gf_size> inline __attribute__((always_inline))  void g_function_proba_in(
          symbols_t * dst,   // the data to be computed for the left side of the graph
    const symbols_t * src_a, // the upper value set from the right side of the graph
    const symbols_t * src_b, // the lower value set from the right side of the graph
    const uint16_t*   src_c, // the computed symbols coming from the left side of the graph
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        for (int i = 0; i < gf_size; i += 1)
        {
            const int idx   = src_c[s] ^ i;
            dst[s].value[idx] = src_a[s].value[i] * src_b[s].value[idx];
        }
        normalize<gf_size>(dst[s].value); // temporal
        dst[s].is_freq = false;
    }
}

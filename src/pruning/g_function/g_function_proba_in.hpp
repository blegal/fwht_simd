#pragma once

#include "utilities/utility_functions.hpp"
#include "fwht/fwht.hpp"

// #define debug_g_function
template <int gf_size>
void g_function_proba_in(
    symbols_t * dst,   // the data to be computed for the left side of the graph
    symbols_t * src_a, // the upper value set from the right side of the graph
    symbols_t * src_b, // the lower value set from the right side of the graph
    uint32_t    src_c  // the computed symbols coming from the left side of the graph
) {
    //
    //  Process the mulitplication to support precomputed symbols
    //
#if _AUTO_CHECK_
    if (src_a->is_freq == true) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    if (src_b->is_freq == true) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
#endif
    // Abdallah computations ...
    for (int i = 0; i < int(gf_size); i++) {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
        //        dst->gf   [i  ] = idx;//src_a->gf   [i]; // to be removed !
    }

    normalize<gf_size>(dst->value); // temporal
    dst->is_freq = false;
}

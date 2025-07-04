#pragma once
//
//
//
//
//
#include "definitions/const_config_GF64_N64.hpp"
#include "features/archi.hpp"
//
//
//
//
//
extern int frozen_symbols[64];
//
//
//
//
//
// #define debug_leaf
template <int gf_size>
void leaf_node(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id) {
#if defined(debug_leaf)
    printf("-> final_node(%d) : frozen = %d\n", symbol_id, frozen_symbols[symbol_id]);
#endif
    //
    // Switch from frequency to time domain if needed
    //
    if (frozen_symbols[symbol_id] == true) {
#if defined(debug_leaf)
        const int max_index = argmax<gf_size>(var->value);
        printf("\e[1;31m[Leaf : %2d - ArgMax = %2d] FROZEN\e[0m\n", symbol_id, max_index);
        if (var->is_freq == true) // Switch from time to frequency domain
        {
            fwht<gf_size>(var->value);
            normalize<gf_size>(var->value, 0.125);
            normalize<gf_size>(var->value);
            var->is_freq = false;
        }
        show_symbols(var);
#endif
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        //        exit( EXIT_FAILURE );
        return;
    }

    if (var->is_freq) {
#if defined(__ARM_NEON__)
        fwht_neon<gf_size>(var->value);
#elif defined(__AVX2__)
        fwht_avx2<gf_size>(var->value);
#else
        fwht<gf_size>(var->value);
        normalize<gf_size>(var->value, 0.125);
        normalize<gf_size>(var->value);
#endif
        var->is_freq = false;
    }

    const int max_index = argmax<gf_size>(var->value);

#if defined(debug_leaf)
    //  normalize<g f_size>(var->value);
    printf("\e[1;31m[Leaf : %2d - ArgMax = %2d]\e[0m\n", symbol_id, max_index);
    show_symbols(var);
#endif

    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
#if defined(debug_leaf)
    // exit( 0 );
#endif
}
//
//
//
//
//

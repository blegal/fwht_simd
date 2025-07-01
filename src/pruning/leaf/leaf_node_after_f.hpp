#pragma once

#include "definitions/const_config_GF64_N64.hpp"
#include "utilities/utility_functions.hpp"
#include "fwht/fwht.hpp"
#include "fwht/fwht_avx2.hpp"
#include "fwht/fwht_neon.hpp"
#include "fwht/fwht_norm_avx2.hpp"
#include "fwht/fwht_norm_neon.hpp"

extern int frozen_symbols[64];

// #define debug_leaf
template <int gf_size>
inline __attribute__((always_inline))
void leaf_node_after_f(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id) {
#if defined(debug_leaf)
    printf("-> leaf_node_after_f(%d) : frozen = %d\n", symbol_id, frozen_symbols[symbol_id]);
#endif
#if _AUTO_CHECK_
    if (var->is_freq == false) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
#endif
    if (frozen_symbols[symbol_id] == true) {
#if defined(debug_leaf)
        fwht<gf_size>(var->value);
        normalize<gf_size>(var->value, 0.125);
        normalize<gf_size>(var->value);
        var->is_freq = false;
        show_symbols(var);
#endif
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
        //        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        //        exit( EXIT_FAILURE );
    }

    //
    // Switch from frequency to time domain
    //
#if defined(__ARM_NEON__)
    fwht_norm_neon<gf_size>(var->value);
#elif defined(__AVX2__)
    fwht_avx2<gf_size>(var->value);
#else
    fwht<gf_size>(var->value);
    normalize<gf_size>(var->value, 0.125);
    normalize<gf_size>(var->value);
    var->is_freq = false;
#endif

    const int max_index = argmax<gf_size>(var->value);
#if defined(debug_leaf)
    printf("\e[1;31m[Leaf : %2d - ArgMax = %2d]\e[0m\n", symbol_id, max_index);
    show_symbols(var);
#endif

    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//

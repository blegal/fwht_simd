#pragma once
//
//
//
//
//
#include "definitions/const_config_GF64_N64.hpp"
#include "utilities/utility_functions.hpp"
#include "fwht/fwht.hpp"
#include "fwht/fwht_avx2.hpp"
#include "fwht/fwht_neon.hpp"
#include "fwht/fwht_norm_avx2.hpp"
#include "fwht/fwht_norm_neon.hpp"
//
//
//
//
//
template <int gf_size> inline __attribute__((always_inline))
void leaf_node_after_f(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int symbol_id,
    const bool frozen)
{
    if (frozen == true) {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
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

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
    FWHT_NORM<gf_size>(var->value);

    const int max_index = argmax<gf_size>(var->value);

    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//

#pragma once
//
//
//
//
//
#include "definitions/const_config_GF64_N64.hpp"
#include "utilities/utility_functions.hpp"
#include "features/archi.hpp"
//
//
//
//
//
template <int gf_size> inline __attribute__((always_inline))
void leaf_node_after_g(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id,
    const bool frozen) {

    if (frozen == true) {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }

    const int max_index = argmax<gf_size>(var->value);
    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//

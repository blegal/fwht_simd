#pragma once

#include "definitions/custom_types.hpp"

extern int frozen_symbols[64];

template <int gf_size>
inline __attribute__((always_inline)) void leaf_node_rate_0(
    symbols_t *,
    uint16_t * decoded,
    uint16_t * symbols,
    const int  symbol_id) {
    decoded[symbol_id] = 0;
    symbols[symbol_id] = 0;
}

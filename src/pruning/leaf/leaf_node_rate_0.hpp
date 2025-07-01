#pragma once

#include "definitions/const_config_GF64_N64.hpp"
#include "fwht/fwht.hpp"
#include "fwht/fwht_avx2.hpp"
#include "fwht/fwht_neon.hpp"
#include "fwht/fwht_norm_avx2.hpp"
#include "fwht/fwht_norm_neon.hpp"
#include "utilities/utility_functions.hpp"

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

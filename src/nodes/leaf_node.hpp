#pragma once
//
//
//
//
//
#include "../structure.hpp"
#include "../fwht/fwht_avx2.hpp"
#include "../const_config_GF64_N64.hpp"
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
template <int gf_size>
void leaf_node(
    symbols_t* var,
    uint16_t* decoded,
    uint16_t* symbols,
    const int symbol_id)
{
#if defined(__DEBUG__)
    printf("-> final_node(%d) : frozen = %d\n", symbol_id, frozen_symbols[symbol_id]);
#endif
//    show_symbols( var );

    if( frozen_symbols[symbol_id] == -1 )
    {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }
    //
    // Switch from frequency to time domain if needed
    //
    if( var->is_freq ) {
        fwht<gf_size>( var->value );
        var->is_freq = false;
    }
//    show_symbols( var );

    int max_index = 0;
    float max_value = var->value[0];
    for (int i = 1; i < gf_size; i++) {
        if (var->value[i] > max_value) {
            max_value = var->value[i];
            max_index = i;
        }
    }

    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//

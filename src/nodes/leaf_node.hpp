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
    //
    // Switch from frequency to time domain if needed
    //
    if( frozen_symbols[symbol_id] == true )
    {
#if 0        
        printf("\e[1;31m");
        printf("[Pos: %2d]", symbol_id);
        printf("\e[0m ");
#endif
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }

    if( var->is_freq ) {
        fwht<gf_size>( var->value );
        var->is_freq = false;
    }
#if 0
    printf("[Pos: %2d]", symbol_id);
    show_symbols( var );
#endif
    const int max_index = argmax<gf_size>(var->value);

    decoded[symbol_id] = max_index; 
    symbols[symbol_id] = max_index;
}
//
//
//
//
//

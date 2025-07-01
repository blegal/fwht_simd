#pragma once
//
//
//
//
//
#include "../../structure.hpp"
#include "../../fwht/fwht_avx2.hpp"
#include "../../const_config_GF64_N64.hpp"
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
inline __attribute__((always_inline))
void leaf_node_after_g(
    symbols_t* var,
    uint16_t* decoded,
    uint16_t* symbols,
    const int symbol_id)
{
#if _AUTO_CHECK_
    if( var->is_freq == true )
    {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }
#endif
    if( frozen_symbols[symbol_id] == true )
    {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
//        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
//        exit( EXIT_FAILURE );
    }


    const int max_index = argmax<gf_size>(var->value);

    decoded[symbol_id] = max_index; 
    symbols[symbol_id] = max_index;
}

#pragma once
//
//
//
//
//
#include "features/archi.hpp"
#include "definitions/custom_types.hpp"
//
//
//
//
//
extern void local_remove_xors(uint16_t * values, int size);
extern void       remove_xors(uint16_t * values, int size);
//
//
//
//
//
template <int gf_size> void middle_node_pruned_rate_1_after_f(
    symbols_t* __restrict inputs,  // Inputs are the symbols from the channel (from the right)
    uint16_t*  __restrict decoded, // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  __restrict symbols, // Symbols are the ones going from leafs to root (done on the left)
    int        size      // Size is the number of symbols (should be a power of 2)
) {
    for (int i = 0; i < size; i++) {
        FWHT<gf_size>(inputs[i].value);
        const int value  = argmax<gf_size>(inputs[i].value);
        symbols[i] = value;
        decoded[i] = value;
    }
    local_remove_xors(decoded, size);
}
//
//
//
//
//
template <int gf_size> void middle_node_pruned_rate_1_after_g(
    symbols_s<gf_size> * inputs,  // Inputs are the symbols from the channel (from the right)
    uint16_t * decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t * symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int        size      // Size is the number of symbols (should be a power of 2)
) {
    for (int i = 0; i < size; i++)
    {
        const int value = argmax<gf_size>(inputs[i].value);
        symbols[i]      = value;
        decoded[i]      = value;
    }
    local_remove_xors(decoded, size);
}
//
//
//
//
//
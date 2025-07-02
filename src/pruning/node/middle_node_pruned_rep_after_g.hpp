#pragma once

#include "utilities/utility_functions.hpp"
#include "pruning/decoder_pruned.hpp"

template <int gf_size>
void decoder_pruned<gf_size>::middle_node_pruned_rate_1_after_f(
    symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
    uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
    int size,               // Size is the number of symbols (should be a power of 2)
    const int symbol_id)    // Symbol ID is the index of the FIRST symbol in the symbols array
{
    int value = argmax<gf_size>( inputs[size-1].value );
    for(int i = 0; i < size; i++)
    {
        symbols[symbol_id + i] = value;
        decoded[symbol_id + i] = value; // should be corrected (it is systematic solution actually)
    }
}


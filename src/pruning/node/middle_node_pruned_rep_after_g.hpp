#pragma once
//
//
//
//
//
#include "../f_function/f_function_freq_in.hpp"
#include "../g_function/g_function_freq_in.hpp"
#include "../leaf/leaf_node_after_f.hpp"
#include "../leaf/leaf_node_after_g.hpp"
//
//
//
//
//
/*
void local_remove_xors(uint16_t* values, int size)
{
    if( size == 1 )
        return;
    for(int i = 0; i < size/2; i += 1)
        values[i] ^= values[i + size/2];
    local_remove_xors(values,          size/2);
    local_remove_xors(values + size/2, size/2);
}
*/
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
//
//
//
//
//

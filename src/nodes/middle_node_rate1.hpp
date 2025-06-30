#pragma once
//
//
//
//
//
#include "../f_function/f_function.hpp"
#include "../g_function/g_function.hpp"
#include "../nodes/leaf_node.hpp"
//
//
//
//
//
template <int gf_size>
void middle_node_rate1(
    symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
    uint16_t* decoded,      // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t* symbols,      // Symbols are the ones going from leafs to root (done on the left)
    int size,               // Size is the number of symbols (should be a power of 2)
    const int symbol_id)    // Symbol ID is the index of the FIRST symbol in the symbols array
{
#if defined(__DEBUG__)
    printf("- middle_node_rate1(%d, %d)\n", size, symbol_id);
#endif
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    // on pourrait simplifier le code de la feuille car on sais qu'elle n'ait pas gelée
    //
    for (int i = 0; i < size; i++) {
        leaf_node<gf_size>(inputs + i, decoded, symbols, symbol_id + i);
    }

    //
    // 
    //
    for (int i = 0; i < n/2; i++) {
        // j'ai un doute...
        // symbols[i] = symbols[i] ^ symbols[i + n/2]; // to be checked !
    }
    //
    // 
    //
}
//
//
//
//
//

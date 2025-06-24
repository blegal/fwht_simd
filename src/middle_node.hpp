#pragma once
//
//
//
//
//
#include "f_function.hpp"
#include "g_function.hpp"
#include "leaf_node.hpp"
//
//
//
//
//
template <int gf_size>
void middle_node(
    symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
    uint16_t* decoded,      // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t* symbols,      // Symbols are the ones going from leafs to root (done on the left)
    int size,               // Size is the number of symbols (should be a power of 2)
    const int symbol_id)    // Symbol ID is the index of the FIRST symbol in the symbols array
{
#if defined(__DEBUG__)
    printf("- middle_node(%d, %d)\n", size, symbol_id);
#endif
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
#if defined(__DEBUG__)
    printf("- f_function\n");
#endif
    for (int i = 0; i < n; i++) {
        f_function<gf_size>( internal + i, inputs + i, inputs + n + i ); // Example operation
    }
    //
    // 
    //
    if( n == 1 ) {
        leaf_node<gf_size>(internal, decoded, symbols, symbol_id); // If we reach the final node, process it
    }else{
        middle_node<gf_size>(internal, internal + n, decoded, symbols, n, symbol_id); // On descend à gauche
    }
    //
    // 
    //
#if defined(__DEBUG__)
    printf("- g_function\n");
#endif
    for (int i = 0; i < n; i++) {
        g_function<gf_size>(
            internal + i,
            inputs + i,
            inputs + n + i,
            symbols + i); // Example operation
    }
    //
    // 
    //
    if( n == 1 ) {
        leaf_node<gf_size>(internal, decoded, symbols, symbol_id + n); // If we reach the final node, process it
    }else{
        middle_node<gf_size>(internal, internal + n, decoded, symbols,n, symbol_id + n); // On descend à droite
    }
    //
    // 
    //
    for (int i = 0; i < n/2; i++) {
        symbols[i] = symbols[i] ^ symbols[i + n/2]; // to be checked !
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

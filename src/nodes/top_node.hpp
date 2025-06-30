#pragma once
#include "middle_node.hpp"
//
//
//
//
//
template <int gf_size = 64>
void top_node(
    symbols_t* channel,     // Channel symbols are the input symbols (from the right)
    symbols_t* internal,    // internal memory space for intermediate computations
    uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
    const int size)         // Size is the number of symbols (should be a power of 2)
{
#if defined(__DEBUG__)
    printf("top_node(%d)\n", size);
#endif
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>(
            internal     + i,
            channel      + i,
            channel  + n + i);
    }
    //
    // 
    //
    middle_node<gf_size>(
        internal,
        internal + n,
        decoded,
        symbols,
        n,
        0); // On descend à gauche
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>(
            internal     + i,    // memory space for the result
            channel      + i,    // values from the right child
            channel  + n + i,    // values from the right child
            symbols[i]);         // decoded symbols from the left child
    }
    //
    // 
    //
    middle_node<gf_size>(
        internal,
        internal + n,
        decoded,
        symbols,
        n,
        n); // On descend à droite
    //
    // 
    //
    // No H computations as we are at the top node and we have a non systematic code !!!
    //
    // 
    //
}
//
//
//
//
//

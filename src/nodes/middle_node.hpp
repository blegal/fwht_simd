#pragma once
//
//
//
//
//
#include "node_functions/f_function.hpp"
#include "node_functions/g_function.hpp"
#include "../nodes/leaf_node.hpp"
//
//
//
//
//
template <int gf_size>
void middle_node(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
#if defined(__DEBUG__)
    printf("- middle_node(%d, %d)\n +> ", size, symbol_id);
    for (int i = 0; i < size; i++) {
        printf("%d ", frozen_symbols[symbol_id + i]); // Assuming gf[0] is the symbol value
    }
    printf("\n +> ");
    for (int i = 0; i < size; i++) {
        printf("%d ", inputs[i].is_freq); // Assuming gf[0] is the symbol value
    }
    printf("\n");
#endif

    const int n = size / 2; // Assuming size is the number of symbols
    //
    //
    //
#if defined(__DEBUG__)
    printf("- f_function\n");
#endif
    for (int i = 0; i < n; i++) {
        f_function<gf_size>(internal + i, inputs + i, inputs + n + i); // Example operation
    }
    //
    //
    //
    if (n == 1) {
        //      leaf_node_after_f<gf_size>(
        leaf_node<gf_size>(
            internal, // le symbol souple
            decoded,  // les symboles décodés (output du decodeur)
            symbols,  // le tableau des symboles durs
            symbol_id);
    } else {
        middle_node<gf_size>(
            internal,     // les données d'entrée
            internal + n, // la mémoire interne pour les calculs
            decoded,      // les symboles décodés (output du decodeur)
            symbols,      // les symboles durs
            n,            // le nombre de données en entrée
            symbol_id);   // l'identifiant du symbole (à gauche)
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
            symbols[symbol_id + i]); // Example operation
#if 0
        printf("(func_g) internal[%2d] = G[%2d, %2d, %2d]\n", i, i, n + i, symbol_id + i);
#endif
    }
    //
    //
    //
    if (n == 1) {
        //      leaf_node_after_g<gf_size>(
        leaf_node<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    } else {
        middle_node<gf_size>(
            internal,
            internal + n,
            decoded,
            symbols,
            n,
            symbol_id + n);
    }
    //
    //
    //
#if 0
    printf("-> debut du xor = %d\n", n);
#endif
    for (int i = 0; i < n; i++) {
#if 0
        const int before = symbols[symbol_id + i];
#endif
        symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
#if 0
        printf(" %2d [%2d] = %2d [%2d] xor %2d [%2d]\n", 
            symbols[symbol_id +     i], symbol_id + i,
            before,                     symbol_id + i,
            symbols[symbol_id + n + i], symbol_id + n + i);
#endif
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

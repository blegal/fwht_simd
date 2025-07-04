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
void remove_xors(uint16_t * values, int size) {
    if (size == 1) {
        return;
    }
    for (int i = 0; i < size / 2; i += 1) {
        values[i] ^= values[i + size / 2];
    }
    remove_xors(values, size / 2);
    remove_xors(values + size / 2, size / 2);
}
//
//
//
//
//
template <int gf_size>
void middle_node_with_pruning(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 1
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += frozen_symbols[symbol_id + i];
    }
    if (sum == size) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 0\n");
#endif
        for (int i = 0; i < size; i++) {
            symbols[symbol_id + i] = 0;
            decoded[symbol_id + i] = 0;
        }
        return;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 1
    if (sum == 0) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 1\n");
#endif
        for (int i = 0; i < size; i++) {
            int value              = argmax<gf_size>(inputs[i].value);
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = value; // should be corrected (it is systematic solution actually)
#if defined(debug_rate_1)
            printf("-> hard decision [%2d] = %d\n", symbol_id + i, symbols[symbol_id + i]);
#endif
        }
        remove_xors(decoded + symbol_id, size);
        return;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 0
    if( (sum == (size-1)) && (frozen_symbols[symbol_id + (size-1)] == false) ) {
#if defined(debug_rate_1)
        printf("Frozen pruning in REP mode\n");
#endif
        // la decision dure sur size/2-1 = n-1 ???
        if ( inputs[n-1].is_freq )
            fwht<gf_size>( inputs[n-1].value );
        const int value = argmax<gf_size>( inputs[n-1].value );
        for(int i = 0; i < size; i++)
        {
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = 0; // should be corrected (it is systematic solution actually)
#if defined(debug_rate_1)
            printf("-> hard decision [%2d] = %d\n", symbol_id + i, symbols[symbol_id + i]);
#endif
        }
        decoded[symbol_id + (size-1)] = value; // should be corrected (it is systematic solution actually)
#if 0
        if ( (sum == (size-1)) && (frozen_symbols[symbol_id + (size-1)] == false) ) {
            printf("> Sum : %d and Size : %d\n", sum, size);
            printf("> FrozenS : ");
            for (int i = 0; i < size; i++)
                printf("%2d ", frozen_symbols[symbol_id + i]);
            printf("\n");
            printf("> Decoded : ");
            for (int i = 0; i < size; i++)
                printf("%2d ", decoded[i]);
            printf("\n");
            printf("> Symbols : ");
            for (int i = 0; i < size; i++)
                printf("%2d ", symbols[i]);
            printf("\n");
        }
        exit( EXIT_FAILURE );
#endif
        return;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>(internal + i, inputs + i, inputs + n + i); // Example operation
    }
    //
    //
    //
    if (n == 1) {
        leaf_node<gf_size>(
            internal, // le symbol souple
            decoded,  // les symboles décodés (output du decodeur)
            symbols,  // le tableau des symboles durs
            symbol_id);
    } else {
        middle_node_with_pruning<gf_size>(
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
    for (int i = 0; i < n; i++) {
        g_function<gf_size>(
            internal + i,
            inputs + i,
            inputs + n + i,
            symbols[symbol_id + i]); // Example operation
    }
    //
    //
    //
    if (n == 1) {
        leaf_node<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    } else {
        middle_node_with_pruning<gf_size>(
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
    for (int i = 0; i < n; i++) {
        symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
    }
    //
    //
    //
#if 0
    if ( (sum == (size-1)) && (frozen_symbols[symbol_id + (size-1)] == false) ) {
        printf("> Sum : %d and Size : %d\n", sum, size);
        printf("> FrozenS : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", frozen_symbols[symbol_id + i]);
        printf("\n");
        printf("> Decoded : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", decoded[i]);
        printf("\n");
        printf("> Symbols : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", symbols[i]);
        printf("\n");
    }
#endif
}
//
//
//
//
//

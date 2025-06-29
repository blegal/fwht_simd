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
void remove_xors(uint16_t* values, int size)
{
    if( size == 1 )
        return;
    for(int i = 0; i < size/2; i += 1)
        values[i] ^= values[i + size/2];
    remove_xors(values,          size/2);
    remove_xors(values + size/2, size/2);
}
//
//
//
//
//
template <int gf_size>
void middle_node_with_pruning(
    symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
    uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
    int size,               // Size is the number of symbols (should be a power of 2)
    const int symbol_id)    // Symbol ID is the index of the FIRST symbol in the symbols array
{

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 1
    int sum = 0;
    for(int i = 0; i < size; i++) { sum += frozen_symbols[symbol_id + i]; }
    if( sum == size ) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 0\n");
#endif
        for(int i = 0; i < size; i++)
        {
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
    if( sum == 0 ) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 1\n");
#endif
        for(int i = 0; i < size; i++)
        {
            int value = argmax<gf_size>( inputs[i].value );
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

    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>( internal + i, inputs + i, inputs + n + i ); // Example operation
    }
    //
    // 
    //
    if( n == 1 ) {
        leaf_node<gf_size>(
            internal,   // le symbol souple
            decoded,    // les symboles décodés (output du decodeur)
            symbols,    // le tableau des symboles durs
            symbol_id);
    }else{
        middle_node_with_pruning<gf_size>(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)
    }
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>(
            internal + i,
            inputs   + i,
            inputs   + n + i,
            symbols[symbol_id + i]); // Example operation
    }
    //
    // 
    //
    if( n == 1 ) {
        leaf_node<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    }else{
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
}
//
//
//
//
//

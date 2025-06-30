#pragma once
//
//
//
//
//
#include "../f_function/f_function_proba_in.hpp"
#include "../g_function/g_function_proba_in.hpp"
#include "../leaf/leaf_node_after_f.hpp"
#include "../leaf/leaf_node_after_g.hpp"
//
//
//
//
//
template <int gf_size>
void decoder_pruned<gf_size>::middle_node_pruned_after_g(
    symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
    uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
    int size,               // Size is the number of symbols (should be a power of 2)
    const int symbol_id)    // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function_proba_in<gf_size>( internal + i, inputs + i, inputs + n + i ); // Example operation
    }
    //
    // 
    //
    next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
    if( left_edge == RATE_0) {
        middle_node_pruned_rate_0(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)
        }else if( left_edge == RATE_1_FROM_F) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else if( left_edge == RATE_1_FROM_G) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else if( left_edge == LEAF_RATE_0) {
        leaf_node_after_f<gf_size>(
            internal,   // le symbol souple
            decoded,    // les symboles décodés (output du decodeur)
            symbols,    // le tableau des symboles durs
            symbol_id);

    }else if( left_edge == LEAF_RATE_1_FROM_F) {
        leaf_node_after_f<gf_size>(
            internal,   // le symbol souple
            decoded,    // les symboles décodés (output du decodeur)
            symbols,    // le tableau des symboles durs
            symbol_id);

    }else if( left_edge == LEAF_RATE_1_FROM_G) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else if( left_edge == MID_NODE_FROM_F) {
        middle_node_pruned_after_f(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)

    }else if( left_edge == MID_NODE_FROM_G) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else{
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        g_function_proba_in<gf_size>(
            internal + i,
            inputs   + i,
            inputs   + n + i,
            symbols[symbol_id + i]); // Example operation
    }
    //
    // 
    //
    next_node right_edge = f_tree->next_node_status[f_tree_cnt++];
    if( right_edge == RATE_0) {
        middle_node_pruned_rate_0(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)
    }else if( right_edge == RATE_1_FROM_F) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }else if( right_edge == RATE_1_FROM_G) {
        middle_node_pruned_rate_1_after_g(
            internal,
            internal + n,
            decoded,
            symbols,
            n,
            symbol_id + n);
    }else if( right_edge == LEAF_RATE_0) {
        leaf_node_after_g<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    }else if( right_edge == LEAF_RATE_1_FROM_F) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }else if( right_edge == LEAF_RATE_1_FROM_G) {
        leaf_node_after_g<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    }else if( right_edge == MID_NODE_FROM_F) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }else if( right_edge == MID_NODE_FROM_G) {
        middle_node_pruned_after_g(
            internal,
            internal + n,
            decoded,
            symbols,
            n,
            symbol_id + n);
    }else{
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
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

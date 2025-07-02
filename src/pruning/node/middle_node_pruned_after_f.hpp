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
template <int gf_size>
void decoder_pruned<gf_size>::middle_node_pruned_after_f(
    symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
    uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
    int size,               // Size is the number of symbols (should be a power of 2)
    const int symbol_id)    // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols
#if 1
    int sum_l = 0;
    int sum_r = 0;
    for(int i = 0; i < n; i++) {
        sum_l += frozen_symbols[symbol_id + i];
        sum_r += frozen_symbols[symbol_id + i + n];
    }
#endif
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function_freq_in<gf_size>( internal + i, inputs + i, inputs + n + i ); // Example operation
    }
    //
    // 
    //
    next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
    if( left_edge == RATE_0) {
#if defined(_DEBUG_DEC_)
        printf("FF :: RATE_0 (%d)\n", size);
#endif
        if ( sum_l != n ) printf("Error %s %d [%d/%d]\n", __FILE__, __LINE__, sum_l, n);
        middle_node_pruned_rate_0(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)
    }else if( left_edge == RATE_1_FROM_F) {
#if defined(_DEBUG_DEC_)
        printf("FF :: RATE_1_FROM_F (%d)\n", size);
#endif
        if ( sum_l != 0 ) printf("Error %s %d\n", __FILE__, __LINE__);
        middle_node_pruned_rate_1_after_f(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else if( left_edge == RATE_1_FROM_G) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else if( left_edge == LEAF_RATE_0) {
#if defined(_DEBUG_DEC_)
        printf("FF :: LEAF_RATE_0 (%d)\n", size);
#endif
        if ( sum_l != 1 ) printf("Error %s %d\n", __FILE__, __LINE__);
        leaf_node_rate_0<gf_size>(
            internal,   // le symbol souple
            decoded,    // les symboles décodés (output du decodeur)
            symbols,    // le tableau des symboles durs
            symbol_id);

    }else if( left_edge == LEAF_RATE_1_FROM_F) {
#if defined(_DEBUG_DEC_)
        printf("FF :: LEAF_RATE_1_FROM_F (%d)\n", size);
#endif
        if ( sum_l != 0 ) printf("Error %s %d\n", __FILE__, __LINE__);
        leaf_node_after_f<gf_size>(
            internal,   // le symbol souple
            decoded,    // les symboles décodés (output du decodeur)
            symbols,    // le tableau des symboles durs
            symbol_id);

    }else if( left_edge == LEAF_RATE_1_FROM_G) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );

    }else if( left_edge == MID_NODE_FROM_F) {
#if defined(_DEBUG_DEC_)
        printf("FF :: MID_NODE_FROM_F (%d)\n", size);
#endif
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
        g_function_freq_in<gf_size>(
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
#if defined(_DEBUG_DEC_)
        printf("FG :: RATE_0 (%d)\n", size);
#endif
        if ( sum_r != n ) printf("Error %s %d\n", __FILE__, __LINE__);
        middle_node_pruned_rate_0(
            internal,       // les données d'entrée
            internal + n,   // la mémoire interne pour les calculs
            decoded,        // les symboles décodés (output du decodeur)
            symbols,        // les symboles durs
            n,              // le nombre de données en entrée
            symbol_id);     // l'identifiant du symbole (à gauche)
    }else if( right_edge == RATE_1_FROM_F) {
#if defined(_DEBUG_DEC_)
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
#endif
        exit( EXIT_FAILURE );
    }else if( right_edge == RATE_1_FROM_G) {
#if defined(_DEBUG_DEC_)
        printf("FG :: RATE_1_FROM_G (%d)\n", size);
#endif
        if ( sum_r != 0 ) printf("Error %s %d\n", __FILE__, __LINE__);
        middle_node_pruned_rate_1_after_g(
            internal,
            internal + n,
            decoded,
            symbols,
            n,
            symbol_id + n);
    }else if( right_edge == LEAF_RATE_0) {
#if defined(_DEBUG_DEC_)
        printf("FG :: LEAF_RATE_0 (%d)\n", size);
#endif
        if ( sum_r != 1 ) printf("Error %s %d\n", __FILE__, __LINE__);
        leaf_node_rate_0<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    }else if( right_edge == LEAF_RATE_1_FROM_F) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }else if( right_edge == LEAF_RATE_1_FROM_G) {
#if defined(_DEBUG_DEC_)
        printf("FG :: LEAF_RATE_1_FROM_G (%d)\n", size);
#endif
        if ( sum_r != 0 ) printf("Error %s %d\n", __FILE__, __LINE__);
        leaf_node_after_g<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    }else if( right_edge == MID_NODE_FROM_F) {
        printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
        exit( EXIT_FAILURE );
    }else if( right_edge == MID_NODE_FROM_G) {
#if defined(_DEBUG_DEC_)
        printf("FG :: MID_NODE_FROM_G (%d)\n", size);
#endif
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
#if 0
    if ( (sum_l == n) && (sum_r == (n-1)) ) {
        printf("Decoded : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", decoded[i]);
        printf("\n");
        printf("Symbols : ");
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

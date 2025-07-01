#pragma once
//
//
//
//
//
#include "frozen_tree.hpp"
//
//
//
//
//
//#define _DEBUG_DEC_

template <int gf_size = 64>
class decoder_pruned
{
public:
    static frozen_tree* f_tree;
    static int f_tree_cnt;

    static void execute(
        symbols_t* channel,     // Channel symbols are the input symbols (from the right)
        symbols_t* internal,    // internal memory space for intermediate computations
        uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
        const int size);        // Size is the number of symbols (should be a power of 2)

private:
    static void middle_node_pruned_after_f(
        symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
        symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
        uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
        int size,               // Size is the number of symbols (should be a power of 2)
        const int symbol_id);   // Symbol ID is the index of the FIRST symbol in the symbols array

    static void middle_node_pruned_after_g(
        symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
        symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
        uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
        int size,               // Size is the number of symbols (should be a power of 2)
        const int symbol_id);   // Symbol ID is the index of the FIRST symbol in the symbols array

    static void middle_node_pruned_rate_0(
        symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
        symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
        uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
        int size,               // Size is the number of symbols (should be a power of 2)
        const int symbol_id);   // Symbol ID is the index of the FIRST symbol in the symbols array

    static void middle_node_pruned_rate_1_after_g(
        symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
        symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
        uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
        int size,               // Size is the number of symbols (should be a power of 2)
        const int symbol_id);   // Symbol ID is the index of the FIRST symbol in the symbols array

    static void middle_node_pruned_rate_1_after_f(
        symbols_t* inputs,      // Inputs are the symbols from the channel (from the right)
        symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
        uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
        int size,               // Size is the number of symbols (should be a power of 2)
        const int symbol_id);   // Symbol ID is the index of the FIRST symbol in the symbols array

};

template <int gf_size>
int decoder_pruned<gf_size>::f_tree_cnt = 0;

template <int gf_size>
frozen_tree* decoder_pruned<gf_size>::f_tree = nullptr;

//
//
//
#include "leaf/leaf_node_rate_0.hpp"
#include "node/middle_node_pruned_after_f.hpp"
#include "node/middle_node_pruned_after_g.hpp"
#include "node/middle_node_pruned_rate_0.hpp"
#include "node/middle_node_pruned_rate_1_after_g.hpp"
#include "node/middle_node_pruned_rate_1_after_f.hpp"
//
//
//
template <int gf_size>
void decoder_pruned<gf_size>::execute(
    symbols_t* channel,     // Channel symbols are the input symbols (from the right)
    symbols_t* internal,    // internal memory space for intermediate computations
    uint16_t*  decoded,     // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,     // Symbols are the ones going from leafs to root (done on the left)
    const int size)         // Size is the number of symbols (should be a power of 2)
{
    f_tree_cnt = 0;

    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function_proba_in<gf_size>(internal + i, channel + i, channel + n + i);
    }
    //
    // 
    //
    next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
    if (left_edge != MID_NODE_FROM_F) {
        exit( EXIT_FAILURE );
    }
    middle_node_pruned_after_f(
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
        g_function_proba_in<gf_size>(
            internal     + i,    // memory space for the result
            channel      + i,    // values from the right child
            channel  + n + i,    // values from the right child
            symbols[i]);         // decoded symbols from the left child
    }
    //
    // 
    //
    next_node right_edge = f_tree->next_node_status[f_tree_cnt++];
    if( right_edge != MID_NODE_FROM_G) {
        exit( EXIT_FAILURE );
    }
    middle_node_pruned_after_g(
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

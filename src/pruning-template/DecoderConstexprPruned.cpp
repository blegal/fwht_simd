#include "DecoderConstexprPruned.hpp"

/**
 *
 * @param n
 * @param frozen_symb
 */
template <int gf_size>
DecoderConstexprPruned<gf_size>::DecoderConstexprPruned(int n) : N(n) {
    internal = new symbols_t[N];
    symbols  = new uint16_t [N];
}

template <int gf_size>
DecoderConstexprPruned<gf_size>::~DecoderConstexprPruned()
{
    delete[]internal;
    delete[]symbols;
}

// These headers are not used directly but defines template functions and MUST be included here
#include "node/middle_node_pruned_after_f.hpp"        // IWYU pragma: keep
#include "node/middle_node_pruned_after_g.hpp"        // IWYU pragma: keep
#include "node/middle_node_pruned_rate_0.hpp"         // IWYU pragma: keep
#include "node/middle_node_pruned_rate_1_after_f.hpp" // IWYU pragma: keep
#include "node/middle_node_pruned_rate_1_after_g.hpp" // IWYU pragma: keep
#include "node/middle_node_pruned_rep_after_f.hpp"    // IWYU pragma: keep
#include "node/middle_node_pruned_rep_after_g.hpp"    // IWYU pragma: keep

template <int gf_size>
void DecoderConstexprPruned<gf_size>::execute(const symbols_t * channel, uint16_t * decoded)
{
    f_tree_cnt = 0;

    const int n = N / 2; // Assuming size is the number of symbols
    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function_proba_in<gf_size>(internal + i, channel + i, channel + n + i);
    }
    //
    //
    //
    NodeType left_edge = next_node_status[f_tree_cnt++];
    if (left_edge != MID_NODE_FROM_F) {
        exit(EXIT_FAILURE);
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
            internal + i,    // memory space for the result
            channel + i,     // values from the right child
            channel + n + i, // values from the right child
            symbols[i]);     // decoded symbols from the left child
    }
    //
    //
    //
    NodeType right_edge = next_node_status[f_tree_cnt++];
    if (right_edge != MID_NODE_FROM_G) {
        exit(EXIT_FAILURE);
    }
    middle_node_pruned_after_g(
        internal,
        internal + n,
        decoded,
        symbols,
        n,
        n); // On descend à droite

    // No H computations as we are at the top node and we have a non systematic code !!!
}

template class DecoderConstexprPruned<GF>;


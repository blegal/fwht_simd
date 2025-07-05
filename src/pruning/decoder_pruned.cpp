#include "decoder_pruned.hpp"

/**
 *
 * @param n
 * @param frozen_symb
 */
template <int gf_size>
decoder_pruned<gf_size>::decoder_pruned(const int n, const int* frozen_symb)
    : N(n), f_tree_cnt(0), f_tree(nullptr)
{
    internal = new symbols_t[N];
    symbols  = new uint16_t [N];
    frozen   = new uint32_t [N];

    for (int i = 0; i < N; i++) {
        frozen[i] = frozen_symb[i];
    }
}

/**
 *
 */
template <int gf_size>
decoder_pruned<gf_size>::decoder_pruned() :
    N(0), f_tree_cnt(0), f_tree(nullptr)

{
    internal = nullptr;
    symbols  = nullptr;
    frozen   = nullptr;
    printf("(EE) Error we should never be there...\n");
    printf("(EE) %s %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

template <int gf_size>
decoder_pruned<gf_size>::~decoder_pruned()
{
    delete[]internal;
    delete[]symbols;
    delete[]frozen;
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
void decoder_pruned<gf_size>::execute(const symbols_t * channel, uint16_t * decoded)
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
    next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
    if (left_edge != MID_NODE_FROM_F) {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
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
    next_node right_edge = f_tree->next_node_status[f_tree_cnt++];
    if (right_edge != MID_NODE_FROM_G) {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
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
//
//
//
#if _GF_ == 16
    template class decoder_pruned< 16>;
#elif _GF_ == 32
    template class decoder_pruned< 32>;
#elif _GF_ == 64
    template class decoder_pruned< 64>;
#elif _GF_ == 128
    template class decoder_pruned<128>;
#elif _GF_ == 256
    template class decoder_pruned<256>;
#elif _GF_ == 1024
    template class decoder_pruned<512>;
#elif _GF_ == 512
    template class decoder_pruned<1024>;
#endif
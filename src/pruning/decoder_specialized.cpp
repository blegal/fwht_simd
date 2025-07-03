#include "decoder_specialized.hpp"
#include "pruning/f_function/f_function_proba_in.hpp"
#include "pruning/g_function/g_function_proba_in.hpp"

// These headers are not used directly but defines template functions and MUST be included here
#include "node/middle_node_after_f.hpp" // IWYU pragma: keep
#include "node/middle_node_after_g.hpp" // IWYU pragma: keep


/**
 *
 * @param n
 * @param frozen_symb
 */
template <int gf_size>
decoder_specialized<gf_size>::decoder_specialized(
    const int n,
    const int* frozen_symb ) : N(n)
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
decoder_specialized<gf_size>::decoder_specialized() : N(0)
{
    internal = nullptr;
    symbols  = nullptr;
    frozen   = nullptr;
    printf("(EE) Error we should never be there...\n");
    printf("(EE) %s %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

template <int gf_size>
decoder_specialized<gf_size>::~decoder_specialized()
{
    delete[]internal;
    delete[]symbols;
    delete[]frozen;
}

template <int gf_size>
void decoder_specialized<gf_size>::execute(
    symbols_t * channel,  // Channel symbols are the input symbols (from the right)
    uint16_t *  decoded)  // Symbols are the ones going from leafs to root (done on the left)
{
    const int n = N / 2;
    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function_proba_in<gf_size>(
            internal + i,
            channel + i,
            channel + n + i);
    }
    //
    //
    //
    middle_node_after_f(
        internal,
        internal + n,
        decoded,
        symbols,
        n,
        0); // On descend à gauche

    for (int i = 0; i < n; i++) {
        g_function_proba_in<gf_size>(
            internal + i,    // memory space for the result
            channel + i,     // values from the right child
            channel + n + i, // values from the right child
            symbols[i]);     // decoded symbols from the left child
    }

    middle_node_after_g(
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
template class decoder_specialized< 16>;
template class decoder_specialized< 32>;
template class decoder_specialized< 64>;
template class decoder_specialized<128>;
template class decoder_specialized<256>;
template class decoder_specialized<512>;

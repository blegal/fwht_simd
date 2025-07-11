#include "decoder_dedicated.hpp"

/**
 *
 * @param n
 * @param frozen_symb
 */
template <int gf_size>
decoder_dedicated<gf_size>::decoder_dedicated(const int n, const int* frozen_symb)
    : N(n), f_tree_cnt(0), f_tree(nullptr), pruned_tree(_N_)
{
    internal = new symbols_t[N];
    symbols  = new uint16_t [N];
    frozen   = new int      [N];

    for (int i = 0; i < N; i++) {
        frozen[i] = frozen_symb[i];
    }
}

/**
 *
 */
template <int gf_size>
decoder_dedicated<gf_size>::decoder_dedicated() :
    N(0), f_tree_cnt(0), f_tree(nullptr), pruned_tree(_N_)

{
    internal = nullptr;
    symbols  = nullptr;
    frozen   = nullptr;
    printf("(EE) Error we should never be there...\n");
    printf("(EE) %s %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

template <int gf_size>
decoder_dedicated<gf_size>::~decoder_dedicated()
{
    delete[]internal;
    delete[]symbols;
    delete[]frozen;
}

#include "dedicated_execute.hpp"

//
//
//
#if _GF_ == 8
    template class decoder_dedicated< 8>;
#elif _GF_ == 16
    template class decoder_dedicated< 16>;
#elif _GF_ == 32
    template class decoder_dedicated< 32>;
#elif _GF_ == 64
    template class decoder_dedicated< 64>;
#elif _GF_ == 128
    template class decoder_dedicated<128>;
#elif _GF_ == 256
    template class decoder_dedicated<256>;
#elif _GF_ == 512
    template class decoder_dedicated<512>;
#elif _GF_ == 1024
    template class decoder_dedicated<1024>;
#endif
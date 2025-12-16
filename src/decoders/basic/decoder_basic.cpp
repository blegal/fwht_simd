#include "decoder_basic.hpp"
#include "decoders/shared/f_function.hpp"
#include "decoders/shared/g_function.hpp"
//
//
//
//
//
template <int gf_size>
decoder_basic<gf_size>::decoder_basic(const int n, const int* frozen_symb ) : N(n)
{
    internal = new symbols_t[N];
    symbols  = new uint16_t [N];
    frozen   = new uint32_t [N];

    for (int i = 0; i < N; i++) {
        frozen[i] = frozen_symb[i];
    }
}
//
//
//
//
//
template <int gf_size> decoder_basic<gf_size>::decoder_basic() : N(0)
{
    internal = nullptr;
    symbols  = nullptr;
    frozen   = nullptr;
    printf("(EE) Error we should never be there...\n");
    printf("(EE) %s %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}
//
//
//
//
//
template <int gf_size> decoder_basic<gf_size>::~decoder_basic()
{
    delete[] internal;
    delete[] symbols;
    delete[] frozen;
}
//
//
//
//
//
template <int gf_size>
void decoder_basic<gf_size>::execute(symbols_t * channel, uint16_t *  decoded)
{
    const int n = N / 2; // Assuming size is the number of symbols
    //
    for (int i = 0; i < n; i++) {
        f_function_proba_only( internal + i, channel + i, channel + n + i);
    }
    //
    middle_node( internal, internal + n, decoded, symbols, n, 0); // On descend à gauche
    //
    for (int i = 0; i < n; i++) {
        g_function_proba_only( internal + i, channel + i, channel + n + i, symbols[i]);
    }
    //
    middle_node( internal, internal + n, decoded, symbols, n, n); // On descend à droite
    //
    // No H computations as we are at the top node and we have a non systematic code !!!
    //
}
//
//
//
//
//
template <int gf_size> void decoder_basic<gf_size>::middle_node(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    for (int i = 0; i < n; i++) {
        f_function_proba_only(internal + i, inputs + i, inputs + n + i);
    }
    //
    if (n == 1) {
        leaf_node(internal, decoded, symbols, symbol_id);
    } else {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id);
    }
    //
    for (int i = 0; i < n; i++) {
        g_function_proba_only(internal + i, inputs + i, inputs + n + i, symbols[symbol_id + i]);
    }
    //
    if (n == 1) {
        leaf_node(internal, decoded, symbols, symbol_id + n);
    } else {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id + n);
    }
    //
    for (int i = 0; i < n; i++) {
        symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
    }
    //
}
//
//
//
//
//
template <int gf_size>
void decoder_basic<gf_size>::leaf_node(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id) {
    //
    // Switch from frequency to time domain if needed
    //
    if (frozen[symbol_id] == true) {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }
    //
    const int max_index = argmax<gf_size>(var->value);
    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//
template <int gf_size>
void decoder_basic<gf_size>::f_function_proba_only(symbols_t * __restrict dst, symbols_t * __restrict src_a, symbols_t * __restrict src_b)
{
    symbols_t tmp_a = *src_a;
    symbols_t tmp_b = *src_b;

    FWHT_NORM<gf_size>(tmp_a.value);
    FWHT_NORM<gf_size>(tmp_b.value);

    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = tmp_a.value[i] * tmp_b.value[i];
    }

    FWHT_NORM<gf_size>(dst->value);
    normalize<gf_size>(dst->value);
    dst->is_freq = false;
}
//
//
//
//
//
template <int gf_size>
void decoder_basic<gf_size>::g_function_proba_only(
    symbols_t * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_t * __restrict src_a, // the upper value set from the right side of the graph
    symbols_t * __restrict src_b, // the lower value set from the right side of the graph
    uint32_t src_c)               // the computed symbols coming from the left side of the graph
{
    for (size_t i = 0; i < gf_size; i++)
    {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i];
    }
    dst->is_freq = false;

    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = dst->value[i] * src_b->value[i];
    }

    normalize<gf_size>(dst->value); // temporal
    dst->is_freq = false;
}
//
//
//
//
//
#if _GF_ == 8
    template class decoder_basic<  8>;
#elif _GF_ == 16
    template class decoder_basic< 16>;
#elif _GF_ == 32
    template class decoder_basic< 32>;
#elif _GF_ == 64
    template class decoder_basic< 64>;
#elif _GF_ == 128
    template class decoder_basic<128>;
#elif _GF_ == 256
    template class decoder_basic<256>;
#elif _GF_ == 512
    template class decoder_basic<512>;
#elif _GF_ == 1024
    template class decoder_basic<1024>;
#elif _GF_ == 2048
    template class decoder_basic<2048>;
#elif _GF_ == 4096
    template class decoder_basic<4096>;
#endif
//
//
//
//
//

#include "decoder_naive_int32_t.hpp"
#include "./impl/f_function.hpp"
#include "./impl/g_function.hpp"
//
//
//
//
//
template <int gf_size>
symbols_i<gf_size> conversion(const symbols_s<gf_size> s)
{
    symbols_i<gf_size> f;
    for (int i = 0; i < gf_size; i++)
    {
        f.value[i] = s.value[i];
    }
    f.is_freq = false; // les données sont des probabilités
    return f;
}
//
//
//
//
//
template <int gf_size>
decoder_naive_int32_t<gf_size>::decoder_naive_int32_t(const int n, const int* frozen_symb ) : N(n)
{
    symbols    = new uint16_t[N];
    frozen     = new uint32_t[N];

    f_channel  = new symbols_i<gf_size>[N];
    f_internal = new symbols_i<gf_size>[N];

    for (int i = 0; i < N; i++)
    {
        frozen[i] = frozen_symb[i];
    }
}
//
//
//
//
//
template <int gf_size> decoder_naive_int32_t<gf_size>::decoder_naive_int32_t() : N(0)
{
    f_internal = nullptr;
    symbols    = nullptr;
    frozen     = nullptr;
    printf("(EE) Error we should never be there...\n");
    printf("(EE) %s %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}
//
//
//
//
//
template <int gf_size> decoder_naive_int32_t<gf_size>::~decoder_naive_int32_t()
{
    delete[] symbols;
    delete[] frozen;

    delete[] f_channel;
    delete[] f_internal;
}

template <int gf_size> void decoder_naive_int32_t<gf_size>::execute(void* s_channel, uint16_t *  decoded)
{
    symbols_s<gf_size>* channel = static_cast< symbols_s<gf_size>* >(s_channel);
    const int n = N / 2; // Assuming size is the number of symbols
    //
    //
    //
    for (int i = 0; i < N; i++) {
        f_channel[i] = convert_to_symbols_i<gf_size>(channel[i]);
    }
    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>(f_internal + i, f_channel + i, f_channel + n + i);
    }
    //
    //
    //
    middle_node(f_internal, f_internal + n, decoded, symbols, n, 0); // On descend à gauche
    //
    //
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>(f_internal + i, f_channel + i, f_channel + n + i, symbols[i]);
    }
    //
    //
    //
    middle_node(f_internal, f_internal + n, decoded, symbols, n, n); // On descend à droite
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
template <int gf_size> void decoder_naive_int32_t<gf_size>::middle_node(
    symbols_i<gf_size>* inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_i<gf_size>* internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t*  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t*  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int size,            // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    //
    //
    for (int i = 0; i < n; i++)
    {
        f_function<gf_size>(internal + i, inputs + i, inputs + n + i);
    }
    //
    //
    //
    if (n == 1)
    {
        leaf_node(internal, decoded, symbols, symbol_id);
    }
    else
    {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id);
    }
    //
    //
    //
    for (int i = 0; i < n; i++)
    {
        g_function<gf_size>(internal + i, inputs + i, inputs + n + i, symbols[symbol_id + i]);
    }
    //
    //
    //
    if (n == 1)
    {
        leaf_node(internal, decoded, symbols, symbol_id + n);
    }
    else
    {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id + n);
    }
    //
    //
    //
    for (int i = 0; i < n; i++)
    {
        symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
    }
    //
    //
    //
}
template <int gf_size> void decoder_naive_int32_t<gf_size>::leaf_node(
    symbols_i<gf_size> * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id)
{
    //
    // Switch from frequency to time domain if needed
    //
    if (frozen[symbol_id] == true)
    {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }

    if (var->is_freq)
    {
/*
        const float factor = norm_factor_lwht<gf_size>();
        i_normalize<gf_size>(var->value, factor);
        i_normalize<gf_size>(var->value, factor);
*/
        fwht<gf_size>(var->value);
        var->is_freq = false;
    }

    const int max_index = i_argmax<gf_size>(var->value);
    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//
template class decoder_naive_int32_t<8>;
template class decoder_naive_int32_t<16>;
template class decoder_naive_int32_t<32>;
template class decoder_naive_int32_t<64>;
template class decoder_naive_int32_t<128>;
template class decoder_naive_int32_t<256>;
template class decoder_naive_int32_t<512>;
template class decoder_naive_int32_t<1024>;
template class decoder_naive_int32_t<2048>;
template class decoder_naive_int32_t<4096>;
//
//
//
//
//

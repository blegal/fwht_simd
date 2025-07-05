#include "decoder_naive_pruning.hpp"
#include "node_functions/f_function.hpp"
#include "node_functions/g_function.hpp"
//
//
//
//
//
void remove_xors(uint16_t * values, int size) {
    if (size == 1) {
        return;
    }
    for (int i = 0; i < size / 2; i += 1) {
        values[i] ^= values[i + size / 2];
    }
    remove_xors(values, size / 2);
    remove_xors(values + size / 2, size / 2);
}
//
//
//
//
//
template <int gf_size>
decoder_naive_pruning<gf_size>::decoder_naive_pruning(
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
//
//
//
//
//
template <int gf_size>
decoder_naive_pruning<gf_size>::decoder_naive_pruning() : N(0)
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
template <int gf_size>
decoder_naive_pruning<gf_size>::~decoder_naive_pruning()
{
    delete[]internal;
    delete[]symbols;
    delete[]frozen;
}
//
//
//
//
//
template <int gf_size>
void decoder_naive_pruning<gf_size>::execute(symbols_t * channel, uint16_t *  decoded)
{
    const int n = N / 2; // Assuming size is the number of symbols
    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>( internal + i, channel + i, channel + n + i);
    }
    //
    //
    //
    middle_node_with_pruning( internal, internal + n, decoded, symbols, n, 0);
    //
    //
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>( internal + i, channel + i, channel + n + i, symbols[i]);
    }
    //
    //
    //
    middle_node_with_pruning( internal, internal + n, decoded, symbols, n, n);
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
template <int gf_size>
void decoder_naive_pruning<gf_size>::middle_node_with_pruning(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 1
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += frozen[symbol_id + i];
    }
    if (sum == size) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 0\n");
#endif
        for (int i = 0; i < size; i++) {
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
    if (sum == 0) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 1\n");
#endif
        for (int i = 0; i < size; i++) {
            int value              = argmax<gf_size>(inputs[i].value);
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

    ///////////////////////////////////////////////////////////////////////////////////////
    //
    //
    // REPETITION NODE !
    //
#if 1
    if( (sum == (size-1)) && (frozen[symbol_id + (size-1)] == false) ) {
#if defined(debug_rate_1)
        printf("Frozen pruning in REP mode [%d::%d]\n", symbol_id, size);
#endif
        // la decision dure sur size/2-1 = n-1 ???
        for(int i = 0; i < size; i++) {
            if ( inputs[i].is_freq == true ) {
                fwht_norm<gf_size>( inputs[i].value );
                inputs[i].is_freq = false;
            }
        }

        float temp[gf_size];
        for (int j = 0; j < gf_size; j++)
            temp[j] = inputs[0].value[j] * inputs[1].value[j];

        for(int i = 2; i < size; i++){
            if( (i & 0x1) == 1)
            normalize<gf_size>( temp );
            for (int j = 0; j < gf_size; j++)
                temp[j] *= inputs[i].value[j] ;
        }

        const int value = argmax<gf_size>( temp );

        for(int i = 0; i < size; i++)
        {
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = 0; // should be corrected (it is systematic solution actually)
        }
        decoded[symbol_id + (size-1)] = value; // should be corrected (it is systematic solution actually)
        return ;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    //
    //
    // SINGLE PARITY NODE
    //
#if 0
    int check_node = 0;
    if( (sum == 1) && (frozen_symbols[symbol_id] == true) ) {
#define debug_rate_spc
#if defined(debug_rate_spc)
        printf("Frozen pruning in SPC mode [%d::%d]\n", symbol_id, size);
#endif

        for(int i = 0; i < size; i++)
            if ( inputs[i].is_freq == true ) {
                fwht<gf_size>( inputs[i].value );
                inputs[i].is_freq = false;
            }

        for (int i = 0; i < size; i++) {
            int value              = argmax<gf_size>(inputs[i].value);
            check_node            ^= value;
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = value; // should be corrected (it is systematic solution actually)
        }
#if defined(debug_rate_spc)
        if ( check_node == 0 ) {
            printf("-> CN equation is validated !\n");
            remove_xors(decoded + symbol_id, size);
            return;
        } else {
            printf("-> CN equation is NOT validated !\n");
            remove_xors(decoded + symbol_id, size);
            for (int j = 0; j < size; j++) {
                printf("  - symbol [%d :: %d] (%f) - Un = %d\n", j, symbols[symbol_id + j], inputs[j].value[symbols[symbol_id + j]], decoded[j + symbol_id]);
            }
            for (int j = 0; j < size; j++)
                show_symbols< gf_size >( inputs[j].value );
        }
#endif
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>(internal + i, inputs + i, inputs + n + i); // Example operation
    }
    //
    //
    //
    if (n == 1) {
        leaf_node( internal, decoded, symbols, symbol_id);
    } else {
        middle_node_with_pruning( internal, internal + n, decoded, symbols, n, symbol_id);
    }
    //
    //
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>( internal + i, inputs + i, inputs + n + i, symbols[symbol_id + i]);
    }
    //
    //
    //
    if (n == 1) {
        leaf_node( internal, decoded, symbols, symbol_id + n);
    } else {
        middle_node_with_pruning( internal, internal + n, decoded, symbols, n, symbol_id + n);
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
template <int gf_size>
void decoder_naive_pruning<gf_size>::middle_node_rate0(
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    //
    //
    for (int i = 0; i < n; i += 1) {
        symbols[symbol_id + i] = 0; // to be checked !
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
template <int gf_size>
void decoder_naive_pruning<gf_size>::middle_node_rate1(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    //
    //
    // on pourrait simplifier le code de la feuille car on sais qu'elle n'ait pas gelée
    //
    for (int i = 0; i < size; i++) {
        leaf_node(inputs + i, decoded, symbols, symbol_id + i);
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
template <int gf_size>
void decoder_naive_pruning<gf_size>::leaf_node(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id)
{
    if (frozen[symbol_id] == true) {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }

    if (var->is_freq) {
        FWHT_NORM<gf_size>(var->value);
        var->is_freq = false;
    }
    const int max_index = argmax<gf_size>(var->value);
    decoded[symbol_id] = max_index;
    symbols[symbol_id] = max_index;
}
//
//
//
//
//
#if _GF_ == 16
    template class decoder_naive_pruning< 16>;
#elif _GF_ == 32
    template class decoder_naive_pruning< 32>;
#elif _GF_ == 64
    template class decoder_naive_pruning< 64>;
#elif _GF_ == 128
    template class decoder_naive_pruning<128>;
#elif _GF_ == 256
    template class decoder_naive_pruning<256>;
#elif _GF_ == 512
    template class decoder_naive_pruning<512>;
#elif _GF_ == 1024
    template class decoder_naive_pruning<1024>;
#endif
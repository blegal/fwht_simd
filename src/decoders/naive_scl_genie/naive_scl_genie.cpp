#include "naive_scl_genie.hpp"
#include "f_function.hpp"
#include "g_function.hpp"
//
//
//
//
//
template <int gf_size>
naive_scl_genie<gf_size>::naive_scl_genie(const int n, const int * frozen_symb, const int L) : N(n), nLists(L) {
    channel  = new symbols_t[N];
    internal = new symbols_t[N];
    symbols  = new uint16_t[N];
    frozen   = new uint32_t[N];
    results  = new uint16_t[N]; // les données pour le mode genie aided !

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
naive_scl_genie<gf_size>::naive_scl_genie() : N(0), n_corrections(0), nLists(0) {
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
naive_scl_genie<gf_size>::~naive_scl_genie() {
    delete[] channel;
    delete[] internal;
    delete[] symbols;
    delete[] frozen;
    delete[] results;
}

template <int gf_size>
void naive_scl_genie<gf_size>::execute(void * s_channel, uint16_t * decoded) {
    //
    //
    //
    n_corrections = 0;
    //
    //
    //
    symbols_s<gf_size> * i_channel = static_cast<symbols_s<gf_size> *>(s_channel);
    for (int i = 0; i < N; i++) {
        channel[i] = convert_to_symbols_t(i_channel[i].value, gf_size, false);
    }
    const int n = N / 2; // Assuming size is the number of symbols
    //
    for (int i = 0; i < n; i++) {
        f_function(internal + i, channel + i, channel + n + i);
    }
    //
    middle_node(internal, internal + n, decoded, symbols, n, 0); // On descend à gauche
    //
    for (int i = 0; i < n; i++) {
        g_function(internal + i, channel + i, channel + n + i, symbols[i]);
    }
    //
    middle_node(internal, internal + n, decoded, symbols, n, n); // On descend à droite
    //
    //
    //
#if 0
    if ( n_corrections != 0 ) {
        printf("n_corrections %d\n", n_corrections);
    }
#endif
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
void naive_scl_genie<gf_size>::middle_node(
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
        f_function(internal + i, inputs + i, inputs + n + i);
    }
    //
    if (n == 1) {
        leaf_node(internal, decoded, symbols, symbol_id);
    } else {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id);
    }
    //
    for (int i = 0; i < n; i++) {
        g_function(internal + i, inputs + i, inputs + n + i, symbols[symbol_id + i]);
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
template <int gf_size>
void naive_scl_genie<gf_size>::leaf_node(
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

    if (var->is_freq) {
        FWHT<gf_size>(var->value);
        var->is_freq = false;
    }

    int max_index = argmax<gf_size>(var->value);
#if 1
    if ( results[symbol_id] != max_index ) {
        var->value[max_index] = 0;
        int max_index2 = argmax<gf_size>(var->value);
        max_index = max_index2;
        //if ( results[symbol_id] != max_index )
        //    std::cout << "(DD) SC-L=2 saved an error !" << std::endl;
        n_corrections += 1;
    }
#elif 0
    if ( results[symbol_id] != max_index ) {
        var->value[max_index] = 0;                  // L=2
        max_index = argmax<gf_size>(var->value);
        if ( results[symbol_id] != max_index ) {
            var->value[max_index] = 0;              // L=3
            max_index = argmax<gf_size>(var->value);
            if ( results[symbol_id] != max_index ) {
                var->value[max_index] = 0;          // L=4
                max_index = argmax<gf_size>(var->value);
            }
        }
    }
#endif
    decoded[symbol_id]  = max_index;
    symbols[symbol_id]  = max_index;
}
//
//
//
//
//
#if (_GF_ == 8) || defined(ALL_GFs)
    template class naive_scl_genie<8>;
#endif
#if (_GF_ == 16) || defined(ALL_GFs)
    template class naive_scl_genie<16>;
#endif
#if (_GF_ == 32) || defined(ALL_GFs)
    template class naive_scl_genie<32>;
#endif
#if (_GF_ == 64) || defined(ALL_GFs)
    template class naive_scl_genie<64>;
#endif
#if (_GF_ == 128) || defined(ALL_GFs)
    template class naive_scl_genie<128>;
#endif
#if (_GF_ == 256) || defined(ALL_GFs)
    template class naive_scl_genie<256>;
#endif
#if (_GF_ == 512) || defined(ALL_GFs)
    template class naive_scl_genie<512>;
#endif
#if (_GF_ == 1024) || defined(ALL_GFs)
    template class naive_scl_genie<1024>;
#endif
#if (_GF_ == 2048) || defined(ALL_GFs)
    template class naive_scl_genie<2048>;
#endif
#if (_GF_ == 4096) || defined(ALL_GFs)
    template class naive_scl_genie<4096>;
#endif
//
//
//
//
//

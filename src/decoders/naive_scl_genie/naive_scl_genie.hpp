#pragma once
#include "decoders/decoder.hpp"
#include "definitions/custom_types.hpp"
#include "features/fwht/fwht_counter.hpp"
//
//
//
//
//
template <int gf_size>
class naive_scl_genie : public decoder {
public:
    naive_scl_genie();
    naive_scl_genie(const int n, const int* frozen_symb, const int L = 2);
    ~naive_scl_genie();

    virtual void execute(void * channel, uint16_t * decoded);
    virtual int  GF() { return gf_size; }
    virtual void setResult(const uint16_t * decoded) {
        for (int i = 0; i < N; i++) {
            results[i] = decoded[i];
        }
    }

private:
    void middle_node(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void leaf_node(
        symbols_t * var,
        uint16_t *  decoded,
        uint16_t *  symbols,
        const int   symbol_id);

    void g_function(
        symbols_t *__restrict dst,	 // the data to be computed for the left side of the graph
        symbols_t *__restrict src_a, // the upper value set from the right side of the graph
        symbols_t *__restrict src_b, // the lower value set from the right side of the graph
        uint32_t src_c);

    void f_function(
        symbols_t * __restrict dst,
        symbols_t * __restrict src_a,
        symbols_t * __restrict src_b);

    symbols_t * channel;
    symbols_t * internal;
    uint16_t *  symbols;
    uint32_t *  frozen;
    uint16_t *  results; // les résultats attendus pour le mode GENIE

    const int N;

    //
    // Debug statistics
    //
    int n_corrections;
    int nLists;
};
//
//
//
//
//

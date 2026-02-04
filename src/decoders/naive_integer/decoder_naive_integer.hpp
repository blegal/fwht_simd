#pragma once
#include "decoders/decoder.hpp"
#include "definitions/custom_types.hpp"
//
//
//
//
//
template <int gf_size>
class decoder_naive_integer : public decoder {
public:
    decoder_naive_integer();
    decoder_naive_integer(const int n, const int * frozen_symb);
    ~decoder_naive_integer();

    void        execute(void * s_channel, uint16_t * decoded);
    virtual int GF() { return gf_size; }

private:
    void middle_node(
        symbols_i32<gf_size> * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_i32<gf_size> * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *             decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *             symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int                    size,     // Size is the number of symbols (should be a power of 2)
        const int              symbol_id);            // Symbol ID is the index of the FIRST symbol in the symbols array

    void leaf_node(
        symbols_i32<gf_size> * var,
        uint16_t *             decoded,
        uint16_t *             symbols,
        const int              symbol_id);

    symbols_i32<gf_size> * f_channel;
    symbols_i32<gf_size> * f_internal;

    uint16_t * symbols;
    uint32_t * frozen;

    const int N;
};
//
//
//
//
//

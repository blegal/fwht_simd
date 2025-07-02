#pragma once

#include "definitions/custom_types.hpp"
#include "pruning/f_function/f_function_proba_in.hpp"
#include "pruning/g_function/g_function_proba_in.hpp"
#include <vector>

template <int gf_size>
class decoder_specialized {
public:
    decoder_specialized();
    decoder_specialized(const int n, const int* frozen_symb);


    void execute(
        symbols_t * channel,   // Channel symbols are the input symbols (from the right)
        uint16_t *  decoded);  // Symbols are the ones going from leafs to root (done on the left)

private:
    void middle_node_after_f(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void middle_node_after_g(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

private:
    std::vector<symbols_t> internal;
    std::vector<uint16_t>  symbols;
    std::vector<uint32_t>  frozen;

    const int N;
};

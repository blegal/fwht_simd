#pragma once

#include "definitions/custom_types.hpp"
#include "frozen_tree.hpp"
#include <vector>

template <int gf_size>
class decoder_pruned {
public:
    decoder_pruned();
    decoder_pruned(const int n, const int* frozen_symb);

    void execute(const symbols_t * channel, uint16_t* decoded);

private:
    void middle_node_pruned_after_f(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void middle_node_pruned_after_g(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void middle_node_pruned_rate_0(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void middle_node_pruned_rate_1_after_g(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void middle_node_pruned_rate_1_after_f(
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
    int           f_tree_cnt;
public:
    frozen_tree * f_tree;
};

#pragma once
//
//
//
//
//
#include "../decoder_specialized_pruning.hpp"
#include "decoders/dedicated/fix_xor_list.hpp"
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
void decoder_specialized_pruning<gf_size>::middle_node_pruned_spc_after_f(
    symbols_s<gf_size> * __restrict inputs,  // Inputs are the symbols from the channel (from the right)
    uint16_t * __restrict decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t * __restrict symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int        size,     // Size is the number of symbols (should be a power of 2)
    const int  symbol_id) // Symbol ID is the index of the FIRST symbol in the symbols array
{
    //
    for(int i = 0; i < size; i++) {
        FWHT<gf_size>(inputs[i].value);
    }
    //
    int check_node = 0;
    uint16_t arg_1[512];
    for (int i = 0; i < size; i++) {
        int value              = argmax<gf_size>(inputs[i].value);
        check_node            ^= value; // on xor le symbole
        symbols[symbol_id + i] = value; // on memorise le symbole (car si le syndrome est OK, c'est fait)
        decoded[symbol_id + i] = value; // on memorise le symbole (car si le syndrome est OK, c'est fait)
        arg_1  [i]             = value; // on le met aussi de coté au cas ou...
    }
    //
    if ( check_node == 0 ) {
        remove_xors(decoded + symbol_id, size);
        return;
    }
    //
    uint16_t arg_2[512];
    for (int j = 0; j < size; j++) {
        arg_2[j] = argmax2<gf_size>(inputs[j].value, arg_1[j]);
    }
    //
    fix_xor_list(arg_1, arg_2, inputs, size);
    //
    for (int j = 0; j < size; j++) {
        symbols[symbol_id + j] = arg_1[j];
        decoded[symbol_id + j] = arg_1[j]; // should be corrected (it is systematic solution actually)
    }
    //
    remove_xors(decoded + symbol_id, size);
    //
}
//
//
//
//
//
template <int gf_size>
void decoder_specialized_pruning<gf_size>::middle_node_pruned_spc_after_g(
    symbols_s<gf_size> * __restrict inputs, // Inputs are the symbols from the channel (from the right)
    uint16_t * __restrict decoded, // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t * __restrict symbols, // Symbols are the ones going from leafs to root (done on the left)
    int       size,                // Size is the number of symbols (should be a power of 2)
    const int symbol_id)           // Symbol ID is the index of the FIRST symbol in the symbols array
{
    //
    //
    int      check_node = 0;
    uint16_t arg_1[512];
    for (int i = 0; i < size; i++)
    {
        int value = argmax<gf_size>(inputs[i].value);
        check_node ^= value;
        symbols[i + symbol_id] = value;
        decoded[i + symbol_id] = value; // should be corrected (it is systematic solution actually)
        arg_1[i]               = value;
    }
    //
    if (check_node == 0)
    {
        remove_xors(decoded + symbol_id, size);
        return;
    }
    //
    uint16_t arg_2[512];
    for (int j = 0; j < size; j++)
    {
        arg_2[j] = argmax2<gf_size>(inputs[j].value, arg_1[j]);
    }
    //
    fix_xor_list(arg_1, arg_2, inputs, size);
    //
    for (int j = 0; j < size; j++)
    {
        symbols[j + symbol_id] = arg_1[j];
        decoded[j + symbol_id] = arg_1[j]; // should be corrected (it is systematic solution actually)
    }
    //
    remove_xors(decoded + symbol_id, size);
    //
}
//
//
//
//
//

#pragma once
//
//
//
//
//
#include "../decoder_specialized_pruning_integer.hpp"
#include "features/archi.hpp"
#include "utilities/utility_functions.hpp"
//
//
//
//
//
template <int gf_size>
void decoder_specialized_pruning_integer<gf_size>::middle_node_pruned_rate_1_after_f(
	symbols_i32<gf_size> *__restrict inputs, // Inputs are the symbols from the channel (from the right)
	symbols_i32<gf_size> *,					 // Internal nodes are the symbols computed during the process (to the left)
	uint16_t *__restrict decoded,			 // Decoded symbols are the final output of the decoder (done on the left)
	uint16_t *__restrict symbols,			 // Symbols are the ones going from leafs to root (done on the left)
	int size,								 // Size is the number of symbols (should be a power of 2)
	const int symbol_id)					 // Symbol ID is the index of the FIRST symbol in the symbols array
{
	for (int i = 0; i < size; i++)
	{
		I32_FWHT<gf_size>(inputs[i].value);
		LZC_normalize<gf_size, I_type::NBITS>(inputs[i].value);
#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 1;
#endif
		const int value = f_argmax<gf_size>(inputs[i].value);
		symbols[symbol_id + i] = value;
		decoded[symbol_id + i] = value;
	}
	local_remove_xors(decoded + symbol_id, size);
}
//
//
//
//
//
template <int gf_size>
void decoder_specialized_pruning_integer<gf_size>::middle_node_pruned_rate_1_after_g(
	symbols_i32<gf_size> *__restrict inputs, // Inputs are the symbols from the channel (from the right)
	symbols_i32<gf_size> *,					 // Internal nodes are the symbols computed during the process (to the left)
	uint16_t *__restrict decoded,			 // Decoded symbols are the final output of the decoder (done on the left)
	uint16_t *__restrict symbols,			 // Symbols are the ones going from leafs to root (done on the left)
	int size,								 // Size is the number of symbols (should be a power of 2)
	const int symbol_id)					 // Symbol ID is the index of the FIRST symbol in the symbols array
{
	for (int i = 0; i < size; i++)
	{
		const int value = f_argmax<gf_size>(inputs[i].value);
		symbols[symbol_id + i] = value;
		decoded[symbol_id + i] = value;
	}
	local_remove_xors(decoded + symbol_id, size);
}
//
//
//
//
//

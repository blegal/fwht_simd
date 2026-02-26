#pragma once
//
//
//
//
//
#include "features/archi.hpp"
#include "utilities/utility_functions.hpp"
//
//
//
//
//

template <int gf_size>
void proba_zero_removal(int32_t *s1)
{
	for (int i = 0; i < gf_size; i++)
	{
		if (s1[i] <= 0)
		{
			s1[i] = 1;
		}
	}
}
template <int gf_size>
void middle_node_pruned_rep_after_f(
	symbols_i32<gf_size> *__restrict inputs, // Inputs are the symbols from the channel (from the right)
	symbols_i32<gf_size> *,					 // Internal nodes are the symbols computed during the process (to the left)
	uint16_t *__restrict decoded,			 // Decoded symbols are the final output of the decoder (done on the left)
	uint16_t *__restrict symbols,			 // Symbols are the ones going from leafs to root (done on the left)
	int size,								 // Size is the number of symbols (should be a power of 2)
	const int symbol_id						 // Symbol ID is the index of the FIRST symbol in the symbols array
)
{
	for (int i = 0; i < size; i++)
	{
		I32_FWHT<gf_size>(inputs[i].value);
		LZC_normalize<gf_size, I_type::NBITS, BlockType::FP>(inputs[i].value);
		proba_zero_removal<gf_size>(inputs[i].value);
#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 1;
#endif
	}

	int32_t temp[gf_size];
	for (int j = 0; j < gf_size; j++)
		temp[j] = inputs[0].value[j];

	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < gf_size; j++)
			temp[j] *= inputs[i].value[j];
		LZC_normalize<gf_size, I_type::NBITS, BlockType::P>(temp);
		// proba_zero_removal<gf_size>(temp);
	}

	const int value = f_argmax<gf_size>(temp);
	for (int i = 0; i < size; i++)
	{
		symbols[symbol_id + i] = value;
		decoded[symbol_id + i] = 0; // should be corrected (it is systematic solution actually)
	}
	decoded[symbol_id + (size - 1)] = value; // should be corrected (it is systematic solution actually)
}
//
//
//
//
//
template <int gf_size>
void middle_node_pruned_rep_after_g(
	symbols_i32<gf_size> *__restrict inputs, // Inputs are the symbols from the channel (from the right)
											 //    symbols_t* internal,    // Internal nodes are the symbols computed during the process (to the left)
	uint16_t *__restrict decoded,			 // Decoded symbols are the final output of the decoder (done on the left)
	uint16_t *__restrict symbols,			 // Symbols are the ones going from leafs to root (done on the left)
	int size,								 // Size is the number of symbols (should be a power of 2)
	const int symbol_id)
{
	int32_t temp[gf_size];
	for (int j = 0; j < gf_size; j++)
		temp[j] = inputs[0].value[j];

	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < gf_size; j++)
			temp[j] *= inputs[i].value[j];
		LZC_normalize<gf_size, I_type::NBITS, BlockType::P>(temp);
		// proba_zero_removal<gf_size>(temp);
	}

	const int value = f_argmax<gf_size>(temp);

	for (int i = 0; i < size; i++)
	{
		symbols[symbol_id + i] = value;
		decoded[symbol_id + i] = 0; // should be corrected (it is systematic solution actually)
	}
	decoded[symbol_id + (size - 1)] = value; // should be corrected (it is systematic solution actually)
}
//
//
//
//
//

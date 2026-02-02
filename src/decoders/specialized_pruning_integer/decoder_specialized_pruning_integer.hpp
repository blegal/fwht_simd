#pragma once
#include "definitions/custom_types.hpp"

#include "decoders/decoder.hpp"
#include "features/fwht/fwht_counter.hpp"
#include "frozen_tree.hpp"
#include <vector>

template <int gf_size>
class decoder_specialized_pruning_integer : public decoder
{
  public:
	decoder_specialized_pruning_integer();
	decoder_specialized_pruning_integer(const int n, const int *frozen_symb);
	~decoder_specialized_pruning_integer();

	virtual void execute(void *channel, uint16_t *decoded);
	virtual int GF()
	{
		return gf_size;
	}

  private:
	void middle_node_pruned_after_f(
		symbols_i32<gf_size> *inputs,	// Inputs are the symbols from the channel (from the right)
		symbols_i32<gf_size> *internal, // Internal nodes are the symbols computed during the process (to the left)
		uint16_t *decoded,				// Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,				// Symbols are the ones going from leafs to root (done on the left)
		int size,						// Size is the number of symbols (should be a power of 2)
		const int symbol_id);			// Symbol ID is the index of the FIRST symbol in the symbols array

	void middle_node_pruned_after_g(
		symbols_i32<gf_size> *inputs,	// Inputs are the symbols from the channel (from the right)
		symbols_i32<gf_size> *internal, // Internal nodes are the symbols computed during the process (to the left)
		uint16_t *decoded,				// Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,				// Symbols are the ones going from leafs to root (done on the left)
		int size,						// Size is the number of symbols (should be a power of 2)
		const int symbol_id);			// Symbol ID is the index of the FIRST symbol in the symbols array

	void middle_node_pruned_rate_0(
		symbols_i32<gf_size> *inputs,	// Inputs are the symbols from the channel (from the right)
		symbols_i32<gf_size> *internal, // Internal nodes are the symbols computed during the process (to the left)
		uint16_t *decoded,				// Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,				// Symbols are the ones going from leafs to root (done on the left)
		int size,						// Size is the number of symbols (should be a power of 2)
		const int symbol_id);			// Symbol ID is the index of the FIRST symbol in the symbols array

	void middle_node_pruned_rate_1_after_g(
		symbols_i32<gf_size> *inputs,	// Inputs are the symbols from the channel (from the right)
		symbols_i32<gf_size> *internal, // Internal nodes are the symbols computed during the process (to the left)
		uint16_t *decoded,				// Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,				// Symbols are the ones going from leafs to root (done on the left)
		int size,						// Size is the number of symbols (should be a power of 2)
		const int symbol_id);			// Symbol ID is the index of the FIRST symbol in the symbols array

	void middle_node_pruned_rate_1_after_f(
		symbols_i32<gf_size> *inputs,	// Inputs are the symbols from the channel (from the right)
		symbols_i32<gf_size> *internal, // Internal nodes are the symbols computed during the process (to the left)
		uint16_t *decoded,				// Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,				// Symbols are the ones going from leafs to root (done on the left)
		int size,						// Size is the number of symbols (should be a power of 2)
		const int symbol_id);			// Symbol ID is the index of the FIRST symbol in the symbols array

	void middle_node_pruned_spc_after_f(
		symbols_i32<gf_size> *inputs, // Inputs are the symbols from the channel (from the right)
		uint16_t *decoded,			  // Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,			  // Symbols are the ones going from leafs to root (done on the left)
		int size,					  // Size is the number of symbols (should be a power of 2)
		const int symbol_id);		  // Symbol ID is the index of the FIRST symbol in the symbols array

	void middle_node_pruned_spc_after_g(
		symbols_i32<gf_size> *inputs, // Inputs are the symbols from the channel (from the right)
		uint16_t *decoded,			  // Decoded symbols are the final output of the decoder (done on the left)
		uint16_t *symbols,			  // Symbols are the ones going from leafs to root (done on the left)
		int size,					  // Size is the number of symbols (should be a power of 2)
		const int symbol_id);		  // Symbol ID is the index of the FIRST symbol in the symbols array

  private:
	symbols_i32<gf_size> *internal;
	uint16_t *symbols;
	symbols_i32<gf_size> *f_channel;
	symbols_i32<gf_size> *f_internal;

	int *frozen;

	const int N;
	int f_tree_cnt;

	frozen_tree *f_tree;
	frozen_tree pruned_tree;
};

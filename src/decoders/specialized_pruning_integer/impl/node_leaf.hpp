#pragma once
//
//
//
//
//
#include "features/archi.hpp"
//
//
//
//
//
template <int gf_size>
inline __attribute__((always_inline)) void leaf_node_after_f(
	symbols_i32<gf_size> *__restrict var,
	uint16_t *__restrict decoded,
	uint16_t *__restrict symbols,
	const int symbol_id,
	const bool frozen)
{
	if (frozen == true)
	{
		decoded[symbol_id] = 0;
		symbols[symbol_id] = 0;
		return;
	}

	//
	// Switch from frequency to time domain
	//
	I32_FWHT<gf_size>(var->value);
	LZC_normalize<gf_size, I_type::NBITS>(var->value);
#if FWHT_COUNTER_ENABLE
	fwht_call_counter += 1;
#endif

	const int max_index = f_argmax<gf_size>(var->value);

	decoded[symbol_id] = max_index;
	symbols[symbol_id] = max_index;
}
//
//
//
//
//
template <int gf_size>
inline __attribute__((always_inline)) void leaf_node_after_g(
	symbols_i32<gf_size> *__restrict var,
	uint16_t *__restrict decoded,
	uint16_t *__restrict symbols,
	const int symbol_id,
	const bool frozen)
{

	if (frozen == true)
	{
		decoded[symbol_id] = 0;
		symbols[symbol_id] = 0;
		return;
	}

	const int max_index = f_argmax<gf_size>(var->value);
	decoded[symbol_id] = max_index;
	symbols[symbol_id] = max_index;
}
//
//
//
//
//
template <int gf_size>
inline __attribute__((always_inline)) void leaf_node_rate_0(
	uint16_t *__restrict decoded,
	uint16_t *__restrict symbols,
	const int symbol_id)
{
	decoded[symbol_id] = 0;
	symbols[symbol_id] = 0;
}
//
//
//
//
//

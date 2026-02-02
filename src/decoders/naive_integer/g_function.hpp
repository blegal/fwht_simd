#pragma once

#include "features/archi.hpp"

template <int gf_size>
void g_function(
	symbols_i32<gf_size> *__restrict dst,	// the data to be computed for the left side of the graph
	symbols_i32<gf_size> *__restrict src_a, // the upper value set from the right side of the graph
	symbols_i32<gf_size> *__restrict src_b, // the lower value set from the right side of the graph
	uint32_t src_c)							// the computed symbols coming from the left side of the graph
{
	if (src_a->is_freq == true)
	{
		I32_FWHT<gf_size>(src_a->value);
		LZC_normalize<gf_size, NBITS>(src_a->value);
		for (int i = 0; i < gf_size; i++)

			if (src_a->value[i] <= 0)
				src_a->value[i] = 1;

		src_a->is_freq = false;
	}

	if (src_b->is_freq == true)
	{

		I32_FWHT<gf_size>(src_b->value);
		LZC_normalize<gf_size, NBITS>(src_b->value);
		for (int i = 0; i < gf_size; i++)

			if (src_b->value[i] <= 0)
				src_b->value[i] = 1;
		src_b->is_freq = false;
	}

	for (size_t i = 0; i < gf_size; i++)
	{
		const int idx = src_c ^ i;
		dst->value[idx] = (src_a->value[i]) * (src_b->value[idx]);
	}
	LZC_normalize<gf_size, NBITS>(dst->value);

	dst->is_freq = false;
}
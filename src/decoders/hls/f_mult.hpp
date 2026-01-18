#pragma once
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "types.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
inline t_int48b vec_i_mul_f(const t_int24b src_1, const t_int24b src_2)
{
#pragma HLS INLINE
//#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_int48b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < gf_size; i++)
	{
		dst.value[i] = src_1.value[i] * src_2.value[i];
	}
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
inline t_int48b vec_i_mul_g(const t_int24b src_1, const t_int24b src_2, const uint8_t symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	t_uint6b tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab.value
	for (int i = 0; i < gf_size; i += 1)
	{
		tab.value[i] = (symbol ^ i);
	}
	t_int48b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < gf_size; i += 1)
	{
		dst.value[i] = src_1.value[ tab.value[i] ] * src_2.value[i];
	}
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//

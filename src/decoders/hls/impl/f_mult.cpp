//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_mult.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_o_mult vec_i_mul_f(const t_i_mult src_1, const t_i_mult src_2)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_o_mult dst;
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
t_o_mult vec_i_mul_g(const t_i_mult src_1, const t_i_mult src_2, const uint8_t symbol)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	t_uint6b tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab.value
	for (int i = 0; i < gf_size; i += 1)
	{
		tab.value[i] = (symbol ^ i);
	}
	t_o_mult dst;
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

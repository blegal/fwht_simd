#pragma once
//
//
#include "definitions/custom_types.hpp"
#include "features/archi.hpp"
//
//
template <int gf_size>
void f_function_freq_in(
	symbols_i32<gf_size> *__restrict dst,
	const symbols_i32<gf_size> *__restrict src_a,
	const symbols_i32<gf_size> *__restrict src_b,
	const int n_symbols)
{
	for (int s = 0; s < n_symbols; s++)
	{
		for (int i = 0; i < gf_size; i++)
			dst[s].value[i] = src_a[s].value[i] * src_b[s].value[i];
		LZC_normalize<gf_size, NBITS>(dst[s].value);
	}
}
//
//
//
//
//
template <int gf_size>
void f_function_proba_in(
	symbols_i32<gf_size> *__restrict dst,
	const symbols_i32<gf_size> *__restrict src_a,
	const symbols_i32<gf_size> *__restrict src_b,
	const int n_symbols)
{
	for (int s = 0; s < n_symbols; s++)
	{
		symbols_i32<gf_size> tmp_a;
		for (int i = 0; i < gf_size; i++)
			tmp_a.value[i] = src_a[s].value[i];
		I32_FWHT<gf_size>(tmp_a.value);
		LZC_normalize<gf_size, NBITS>(tmp_a.value);
#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 1;
#endif

		symbols_i32<gf_size> tmp_b;
		for (int i = 0; i < gf_size; i++)
			tmp_b.value[i] = src_b[s].value[i];
		I32_FWHT<gf_size>(tmp_b.value);
		LZC_normalize<gf_size, NBITS>(tmp_b.value);
#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 1;
#endif

		//
		// Element-wise multiplication of the two input symbols because we are in frequency domain !
		//
		for (size_t i = 0; i < gf_size; i++)
		{
			dst[s].value[i] = tmp_a.value[i] * tmp_b.value[i]; // TODO : attention au facteur 10x qui est magique !!!
		}
		LZC_normalize<gf_size, NBITS>(dst[s].value);
	}
}
//
//
//
//
//

#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"
#include "hadamard_i32/Hadamard.hpp"

template <int gf_size>
void g_function_zero_removal(int32_t *s1)
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
void g_function_freq_in(
	symbols_i32<gf_size> *__restrict dst,
	symbols_i32<gf_size> *__restrict src_a,
	symbols_i32<gf_size> *__restrict src_b,
	const uint16_t *__restrict src_c,
	const int n_symbols)
{
	for (int s = 0; s < n_symbols; s++)
	{
		const int idx = src_c[s];
		const int32_t *H = get_Hadamard_line<gf_size>(idx);
		for (size_t i = 0; i < gf_size; i++)
			dst[s].value[i] = src_a[s].value[i] * H[i];
		I32_FWHT<gf_size>(dst[s].value);
		I32_FWHT<gf_size>(src_b[s].value);
		LZC_normalize<gf_size, I_type::NBITS, BlockType::FP>(dst[s].value);
		LZC_normalize<gf_size, I_type::NBITS, BlockType::FP>(src_b[s].value);
		g_function_zero_removal<gf_size>(dst[s].value);
		g_function_zero_removal<gf_size>(src_b[s].value);

#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 2;
#endif
		for (size_t i = 0; i < gf_size; i++)
			dst[s].value[i] = dst[s].value[i] * src_b[s].value[i];
		LZC_normalize<gf_size, I_type::NBITS, BlockType::P>(dst[s].value);
	}
}
//
//
//
//
//
template <int gf_size>
void g_function_proba_in(
	symbols_i32<gf_size> *__restrict dst,
	symbols_i32<gf_size> *__restrict src_a,
	symbols_i32<gf_size> *__restrict src_b,
	const uint16_t *__restrict src_c,
	const int n_symbols)
{
	for (int s = 0; s < n_symbols; s++)
	{
		g_function_zero_removal<gf_size>(src_a[s].value);
		g_function_zero_removal<gf_size>(src_b[s].value);
		for (int i = 0; i < gf_size; i += 1)
		{
			const int idx = src_c[s] ^ i;
			dst[s].value[idx] = src_a[s].value[i] * src_b[s].value[idx];
		}
		LZC_normalize<gf_size, I_type::NBITS, BlockType::P>(dst[s].value);
	}
}
//
//
//
//
//
template <int gf_size>
void g_function_freq_in_after_rate_0(
	symbols_i32<gf_size> *__restrict dst,
	symbols_i32<gf_size> *__restrict src_a,
	symbols_i32<gf_size> *__restrict src_b,
	const int n_symbols)
{
	for (int s = 0; s < n_symbols; s++)
	{
		I32_FWHT<gf_size>(src_a[s].value);
		I32_FWHT<gf_size>(src_b[s].value);
		LZC_normalize<gf_size, I_type::NBITS, BlockType::FP>(src_a[s].value);
		LZC_normalize<gf_size, I_type::NBITS, BlockType::FP>(src_b[s].value);
		g_function_zero_removal<gf_size>(src_a[s].value);
		g_function_zero_removal<gf_size>(src_b[s].value);
#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 2;
#endif

		for (int i = 0; i < gf_size; i++)
		{
			const int32_t val = src_a[s].value[i] * src_b[s].value[/*idx*/ i];
			dst[s].value[i] = val;
		}
		LZC_normalize<gf_size, I_type::NBITS, BlockType::P>(dst[s].value);
	}
}
//
//
//
//
//
template <int gf_size>
void g_function_proba_in_after_rate_0(
	symbols_i32<gf_size> *__restrict dst,
	const symbols_i32<gf_size> *__restrict src_a,
	const symbols_i32<gf_size> *__restrict src_b,
	const int n_symbols)
{
	for (int s = 0; s < n_symbols; s++)
	{
		for (int i = 0; i < gf_size; i += 1)
		{
			dst[s].value[i] = src_a[s].value[i] * src_b[s].value[i];
		}
		LZC_normalize<gf_size, I_type::NBITS, BlockType::P>(dst[s].value);
		g_function_zero_removal<gf_size>((dst[s].value));
	}
}
//
//
//
//
//

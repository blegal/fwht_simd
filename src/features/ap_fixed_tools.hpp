//!
//!     Copyright (c) 2020-2023, Bertrand LE GAL
//!     All rights reserved.
//!
//!     Redistribution and use in source and binary forms, with or without
//!     modification, are not permitted with written authorization.
//!
//!
#pragma once
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
#ifndef AP_FIXED_TOOLS
#define AP_FIXED_TOOLS
#include "definitions/custom_types.hpp"
#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>

#define NAIVE_LZC_norm

template <int GF_SIZE, int NBITS1>
void LZC_normalize(int64_t *s1)
{
	const int64_t *s = s1;
	constexpr int64_t max_val = (1LL << (NBITS1 - 1)) - 1;
	constexpr int64_t min_val = -(1LL << (NBITS1 - 1));
	constexpr int F_in = NBITS1 - 1;
	// auto              abs64   = [](int64_t x) -> uint64_t {
	//     if (x == INT64_MIN)
	//         return (1ULL << 63);
	//     return (x < 0) ? (uint64_t) (-x) : (uint64_t) x;
	// };
	uint64_t or_all = 0;
	for (int j = 0; j < GF_SIZE; j++)
		or_all |= (uint64_t)std::abs(s[j]);

	if (or_all == 0)
	{
		for (int j = 0; j < GF_SIZE; j++)
			s1[j] = 0;
		return;
	}
	int msb_pos;

#if defined(__GNUG__)
	msb_pos = 63 - __builtin_clzll(or_all);
#else
	{
		uint64_t x = or_all;
		msb_pos = 0;
		while (x >>= 1)
			msb_pos++;
	}
#endif
	int k_max = msb_pos - F_in;
	int shift = -1 - k_max;
	for (int j = 0; j < GF_SIZE; j++)
	{
		int64_t v = s[j];
		uint64_t v_abs = (uint64_t)std::abs(v);
		int64_t out;
		if (shift < 0)
		{
			int r = -shift;
			out = v >> r;
		}
		else if (shift > 0)
		{
			uint64_t mag = v_abs << shift;
			if (mag >> 63)
				out = (v >= 0) ? max_val : min_val + 1;
			else
				out = (v >= 0) ? (int64_t)mag : -(int64_t)mag;
		}
		else
		{
			out = v;
		}
		s1[j] = out;
	}
}

template <int GF_SIZE, int NBITS1>
void LZC_normalize(int32_t *s1)
{
	const int32_t *s = s1;
	constexpr int F_in = NBITS1 - 1;

	auto abs_val = [](int32_t x) -> uint32_t
	{
		if (x == INT32_MIN)
			return (uint32_t)INT32_MAX + 1;
		return (x < 0) ? (uint32_t)(-x) : (uint32_t)x;
	};

	uint32_t or_all = 0;
	for (int j = 0; j < GF_SIZE; j++)
		or_all |= abs_val(s[j]);

	if (or_all == 0)
	{
		for (int j = 0; j < GF_SIZE; j++)
			s1[j] = 0;
		return;
	}

	int msb_pos;
#if defined(__GNUG__)
	msb_pos = 31 - __builtin_clz(or_all);
#else
	{
		uint32_t x = or_all;
		msb_pos = 0;
		while (x >>= 1)
			msb_pos++;
	}
#endif

	int target_bit = F_in - 1;
	int shift = msb_pos - target_bit;

	for (int j = 0; j < GF_SIZE; j++)
		s1[j] >>= shift;
}

template <int GF_SIZE, int NBITS1>
void LZCnormAfterF(int32_t *s)
{
#ifndef NAIVE_LZC_norm
	LZC_normalize<GF_SIZE, NBITS1>(s);
	return;
#endif

	constexpr int F_in = NBITS1 - 1;
 // BLG SILENT WARNING constexpr int target_bit = F_in - 1;
	constexpr int32_t test_mask = (int32_t)1 << (2 * F_in - 1);
	int32_t first = s[0];
	int shift = (first & test_mask) ? F_in : (F_in - 1);

	for (int j = 0; j < GF_SIZE; j++)
		s[j] >>= shift;
}

template <int GF_SIZE, int NBITS1>
void LZCnormAfterG(int32_t *s)
{
#ifndef NAIVE_LZC_norm
	LZC_normalize<GF_SIZE, NBITS1>(s);
	return;
#endif
	constexpr int F_in = NBITS1 - 1;
	constexpr int target_bit = F_in - 1;

	uint32_t or_all = 0;
	for (int j = 0; j < GF_SIZE; j++)
	{
		int32_t v = s[j];
		or_all |= (v < 0) ? (uint32_t)(-v) : (uint32_t)v;
	}

	int msb_pos;
#if defined(__GNUG__)
	msb_pos = 31 - __builtin_clz(or_all);
#else
	{
		uint32_t x = or_all;
		msb_pos = 0;
		while (x >>= 1)
			msb_pos++;
	}
#endif

	int shift = msb_pos - target_bit;

	for (int j = 0; j < GF_SIZE; j++)
		s[j] >>= shift;
}

template <int GF_SIZE, int NBITS1>
void LZCnormAfterHaramardPF(int32_t *s)
{
#ifndef NAIVE_LZC_norm
	LZC_normalize<GF_SIZE, NBITS1>(s);
	return;
#endif
	constexpr int p = []
	{
		int v = GF_SIZE, r = 0;
		while (v >>= 1)
			++r;
		return r;
	}();
	constexpr int F_in = NBITS1 - 1;
	constexpr int target_bit = F_in - 1;
	constexpr int low_bound = (p > F_in - 1) ? p : (F_in - 1);
	constexpr int high_bound = p + F_in - 1;

	int32_t first = s[0];
	uint32_t masked = first & (((1 << (high_bound + 1)) - 1) & ~((1 << low_bound) - 1));

	int msb_pos;
#if defined(__GNUG__)
	msb_pos = 31 - __builtin_clz(masked);
#else
	{
		uint32_t x = masked;
		msb_pos = low_bound - 1;
		msb_pos++;
	}
#endif

	int shift = msb_pos - target_bit;

	for (int j = 0; j < GF_SIZE; j++)
		s[j] >>= shift;
}

template <int GF_SIZE, int NBITS1>
void LZCnormAfterHaramardFP(int32_t *s)
{
#ifdef NAIVE_LZC_norm
	LZC_normalize<GF_SIZE, NBITS1>(s);
	return;
#endif
	constexpr int p = []
	{
		int v = GF_SIZE, r = 0;
		while (v >>= 1)
			++r;
		return r;
	}();
	constexpr int F_in = NBITS1 - 1;
	constexpr int target_bit = F_in - 1;
	constexpr int low_bound = F_in - 1;
	constexpr int high_bound = p + F_in - 1;

	uint32_t or_all = 0;
	for (int j = 0; j < GF_SIZE; j++)
	{
		int32_t v = s[j];
		or_all |= (v < 0) ? (uint32_t)(-v) : (uint32_t)v;
	}

	if (or_all == 0)
		return;

	uint32_t masked = or_all & (((1 << (high_bound + 1)) - 1) & ~((1 << low_bound) - 1));

	int msb_pos;
#if defined(__GNUG__)
	msb_pos = 31 - __builtin_clz(masked);
#else
	{
		uint32_t x = masked;
		msb_pos = low_bound - 1; // start just below range
		while (x >>= 1)
			msb_pos++;
	}
#endif

	int shift = msb_pos - target_bit;

	for (int j = 0; j < GF_SIZE; j++)
		s[j] >>= shift;
}

// At the top of your file, define the enum
enum class BlockType
{
	P,
	F,
	PF,
	FP
};

template <int GF_SIZE, int NBITS1, BlockType block_type>
void LZC_normalize(int32_t *s)
{
	if constexpr (block_type == BlockType::P)
	{
		LZCnormAfterG<GF_SIZE, NBITS1>(s);
	}
	else if constexpr (block_type == BlockType::F)
	{
		LZCnormAfterF<GF_SIZE, NBITS1>(s);
	}
	else if constexpr (block_type == BlockType::PF)
	{
		LZCnormAfterHaramardPF<GF_SIZE, NBITS1>(s);
	}
	else if constexpr (block_type == BlockType::FP)
	{
		LZCnormAfterHaramardFP<GF_SIZE, NBITS1>(s);
	}
}
#endif
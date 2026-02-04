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
#include <climits>
#include <cmath>
#include <cstdint>

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
	constexpr int32_t max_val = (1LL << (NBITS1 - 1)) - 1;
	constexpr int32_t min_val = -(1LL << (NBITS1 - 1));
	constexpr int F_in = NBITS1 - 1;
	auto abs64 = [](int32_t x) -> uint32_t
	{
		if (x == INT64_MIN)
			return (1ULL << 31);
		return (x < 0) ? (uint32_t)(-x) : (uint32_t)x;
	};
	uint32_t or_all = 0;
	for (int j = 0; j < GF_SIZE; j++)
		or_all |= abs64(s[j]);

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
		uint32_t x = or_all;
		msb_pos = 0;
		while (x >>= 1)
			msb_pos++;
	}
#endif
	int k_max = msb_pos - F_in;
	int shift = -1 - k_max;
	for (int j = 0; j < GF_SIZE; j++)
	{
		int32_t v = s[j];
		uint32_t v_abs = abs64(v);
		int32_t out;
		if (shift < 0)
		{
			int r = -shift;
			out = v >> r;
		}
		else if (shift > 0)
		{
			uint32_t mag = v_abs << shift;
			if (mag >> 31)
				out = (v >= 0) ? max_val : min_val + 1;
			else
				out = (v >= 0) ? (int32_t)mag : -(int32_t)mag;
		}
		else
		{
			out = v;
		}
		s1[j] = out;
	}
}

#endif
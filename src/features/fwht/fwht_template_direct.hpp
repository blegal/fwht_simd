/**
  Copyright (c) "Camille Monière"

  This file is part of LDPC_C_Simulator.

  LDPC_C_Simulator is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <array>

template <class T, size_t size>
constexpr auto make_weight() {
	static_assert(((size >> 1) << 1) == size, "size must be a power of 2.");
	std::array<T, size> w = {0};
	for (size_t j = 0; j < size; j++) {
		w[j] = T((size >> 1) > j) - T(j > (size >> 1));
	}
	return w;
}

template <size_t galois_size>
inline void fwht_restrict(float * __restrict dst, const float * __restrict src) {
	static_assert(galois_size > 1, "galois_size must be over 1.");
	constexpr size_t half_gf = galois_size >> 1;
	static_assert((half_gf << 1) == galois_size, "galois_size must be a power of 2.");
	//constexpr std::array<float, galois_size> weight = make_weight<float, galois_size>();
	float intermediary[galois_size];
	for (size_t j = 0; j < half_gf; j++) {
		const size_t index = j & (half_gf - 1);
		intermediary[j] = src[index] + src[index + half_gf];
	}
	for (size_t j = half_gf; j < galois_size; j++) {
		const size_t index = j & (half_gf - 1);
		intermediary[j] = src[index] - src[index + half_gf];
	}
	if constexpr (galois_size > 1) {
		fwht_restrict<half_gf>(dst, intermediary);
		fwht_restrict<half_gf>(dst + half_gf, intermediary + half_gf);
	}
}

template<>
inline void fwht_restrict<1>(float *__restrict dst, const float *__restrict src) {
	dst[0] = src[0];
}

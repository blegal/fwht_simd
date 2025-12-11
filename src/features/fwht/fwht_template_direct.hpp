/**
  Copyright (c) "Camille Monière"

  This file is part of FDSC-PC_CXX_Simulator.

  FDSC-PC_CXX_Simulator is free software: you can redistribute it and/or modify
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

template <size_t galois_size>
inline void fwht_template_direct(float * inout) {
	static_assert(galois_size > 1, "galois_size must be over 1.");
	constexpr size_t half_gf = galois_size >> 1;
	static_assert((half_gf << 1) == galois_size , "galois_size must be a power of 2.");
	float intermediary[galois_size];
	for (size_t j = 0; j < half_gf; j++) {
		intermediary[j] = inout[j] + inout[j + half_gf];
	}
	for (size_t j = 0; j < half_gf; j++) {
		intermediary[j + half_gf] = inout[j] - inout[j + half_gf];
	}

	fwht_template_direct<half_gf>(intermediary);
	fwht_template_direct<half_gf>(intermediary + half_gf);

	for (size_t j = 0; j < galois_size; j++) {
		inout[j] = intermediary[j];
	}
}

template<>
inline void fwht_template_direct<1>(float * inout) {}

template <size_t galois_size>
inline void fwht_template_direct(float * __restrict dst, const float * __restrict src) {
	static_assert(galois_size > 1, "galois_size must be over 1.");
	constexpr size_t half_gf = galois_size >> 1;
	static_assert((half_gf << 1) == galois_size, "galois_size must be a power of 2.");
	float intermediary[galois_size];
	for (size_t j = 0; j < half_gf; j++) {
		intermediary[j] = src[j] + src[j + half_gf];
	}
	for (size_t j = 0; j < half_gf; j++) {
		intermediary[j + half_gf] = src[j] - src[j + half_gf];
	}

	fwht_template_direct<half_gf>(dst, intermediary);
	fwht_template_direct<half_gf>(dst + half_gf, intermediary + half_gf);
}

template<>
inline void fwht_template_direct<1>(float *__restrict dst, const float *__restrict src) {
	dst[0] = src[0];
}

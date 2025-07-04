/**
  Copyright (c) 2012-2015 "Bordeaux INP, Bertrand LE GAL"
  [http://legal.vvv.enseirb-matmeca.fr]

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
#include "definitions/const_config_GF64_N64.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <uint16_t GF>
inline void fwht(value_type x[]) {
    assert(x != 0);
    assert(true);
    exit(x != NULL); // pour gerer le release mode
}

template <uint16_t GF>
inline void fwht(value_type * dst, const value_type * src) {
    assert(src != nullptr);
    assert(dst != nullptr);
    assert(true);
    exit(src != nullptr); // pour gerer le release mode
}

template <uint16_t GF>
inline void normalize(value_type x[], const value_type fact) {
    for (int i = 0; i < GF; i++)
        x[i] = x[i] * fact;
}

//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
inline void fwht_tuile(const value_type inp[8], value_type outp[8]) {
    value_type L1[8], L2[8];
    L1[0] = inp[0] + inp[4];
    L1[1] = inp[1] + inp[5];
    L1[2] = inp[2] + inp[6];
    L1[3] = inp[3] + inp[7];
    L1[4] = inp[0] - inp[4];
    L1[5] = inp[1] - inp[5];
    L1[6] = inp[2] - inp[6];
    L1[7] = inp[3] - inp[7];

    L2[0] = L1[0] + L1[2];
    L2[2] = L1[0] - L1[2];
    L2[1] = L1[1] + L1[3];
    L2[3] = L1[1] - L1[3];
    L2[4] = L1[4] + L1[6];
    L2[6] = L1[4] - L1[6];
    L2[5] = L1[5] + L1[7];
    L2[7] = L1[5] - L1[7];

    outp[0] = L2[0] + L2[1];
    outp[1] = L2[0] - L2[1];
    outp[2] = L2[2] + L2[3];
    outp[3] = L2[2] - L2[3];
    outp[4] = L2[4] + L2[5];
    outp[5] = L2[4] - L2[5];
    outp[6] = L2[6] + L2[7];
    outp[7] = L2[6] - L2[7];
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<8>(value_type inp[8]) {
    value_type part_1[8];
    for (int i = 0; i < 4; i++) {
        part_1[i]     = inp[i] + inp[i + 4];
        part_1[4 + i] = inp[i] - inp[i + 4];
    }
    fwht_tuile(part_1, inp);
}
//
//
//
template <>
inline void fwht<8>(value_type * dst, const value_type * src) {
    // value_type part_1[8];
    for (int i = 0; i < 4; i++) {
        dst[i]     = src[i] + src[i + 4];
        dst[4 + i] = src[i] - src[i + 4];
    }
    fwht_tuile(dst, dst);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<16>(value_type inp[16]) {
    value_type part_1[8];
    value_type part_2[8];

    for (int i = 0; i < 8; i++)
        part_1[i] = inp[i] + inp[i + 8];
    for (int i = 0; i < 8; i++)
        part_2[i] = inp[i] - inp[i + 8];

    fwht_tuile(part_1, inp + 0);
    fwht_tuile(part_2, inp + 8);
}
//
//
//
template <>
inline void fwht<16>(value_type * dst, const value_type * src) {
    for (int i = 0; i < 8; i++) {
        dst[i]     = src[i] + src[i + 8];
        dst[8 + i] = src[i] - src[i + 8];
    }
    fwht_tuile(dst, dst + 0);
    fwht_tuile(dst + 8, dst + 8);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<32>(value_type inp[32]) {
    value_type part_1[16];
    value_type part_2[16];

    for (int i = 0; i < 16; i++) {
        part_1[i] = inp[i] + inp[i + 16];
        part_2[i] = inp[i] - inp[i + 16];
    }

    fwht<16>(part_1);
    fwht<16>(part_2);

    for (int i = 0; i < 16; i++) {
        inp[i]      = part_1[i];
        inp[16 + i] = part_2[i];
    }
}
//
//
//
template <>
inline void fwht<32>(value_type * dst, const value_type * src) {
    for (int i = 0; i < 16; i++) {
        dst[i]      = src[i] + src[i + 16];
        dst[16 + i] = src[i] - src[i + 16];
    }
    fwht<16>(dst, dst);
    fwht<16>(dst + 16, dst + 16);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<64>(value_type inp[64]) {
    value_type part_1[32];
    value_type part_2[32];

    for (int i = 0; i < 32; i++) {
        part_1[i] = inp[i] + inp[i + 32];
        part_2[i] = inp[i] - inp[i + 32];
    }

    fwht<32>(part_1);
    fwht<32>(part_2);

    for (int i = 0; i < 32; i++) {
        inp[i]      = part_1[i];
        inp[32 + i] = part_2[i];
    }
}
//
//
//
template <>
inline void fwht<64>(value_type * dst, const value_type * src) {
    for (int i = 0; i < 32; i++) {
        dst[i]      = src[i] + src[i + 32];
        dst[32 + i] = src[i] - src[i + 32];
    }
    fwht<32>(dst, dst);
    fwht<32>(dst + 32, dst + 32);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<128>(value_type inp[128]) {
    value_type part_1[64], part_2[64];

    for (int i = 0; i < 64; i++) {
        part_1[i] = inp[i] + inp[i + 64];
        part_2[i] = inp[i] - inp[i + 64];
    }

    fwht<64>(part_1);
    fwht<64>(part_2);

    for (int i = 0; i < 64; i++) {
        inp[i + 0]  = part_1[i];
        inp[i + 64] = part_2[i];
    }
}
//
//
//
template <>
inline void fwht<128>(value_type * dst, const value_type * src) {
    for (int i = 0; i < 64; i++) {
        dst[i]      = src[i] + src[i + 64];
        dst[64 + i] = src[i] - src[i + 64];
    }
    fwht<64>(dst, dst);
    fwht<64>(dst + 64, dst + 64);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<256>(value_type inp[256]) {
    value_type part_1[128];
    value_type part_2[128];

    for (int i = 0; i < 128; i++) {
        part_1[i] = inp[i] + inp[i + 128];
        part_2[i] = inp[i] - inp[i + 128];
    }

    fwht<128>(part_1);
    fwht<128>(part_2);

    for (int i = 0; i < 128; i++) {
        inp[i + 0]   = part_1[i];
        inp[i + 128] = part_2[i];
    }
}
//
//
//
template <>
inline void fwht<256>(value_type * dst, const value_type * src) {
    for (int i = 0; i < 128; i++) {
        dst[i]       = src[i] + src[i + 128];
        dst[128 + i] = src[i] - src[i + 128];
    }
    fwht<128>(dst, dst);
    fwht<128>(dst + 128, dst + 128);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

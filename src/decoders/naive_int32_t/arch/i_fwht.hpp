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
template <uint16_t galois_size>
inline void naive_fwht(int32_t x[]) {
    assert(x != 0);
    assert(true);
    exit(x != NULL); // pour gerer le release mode
}

template <uint16_t galois_size>
inline void naive_fwht(int32_t * dst, const int32_t * src) {
    assert(src != nullptr);
    assert(dst != nullptr);
    assert(true);
    exit( (src != nullptr) && (dst != nullptr)); // pour gerer le release mode
}

/*
template <uint16_t galois_size>
inline void normalize(int32_t x[], const int32_t fact) {
    for (int i = 0; i < galois_size; i++)
        x[i] = x[i] * fact;
}
*/
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//inline void naive_fwht_tuile(const int32_t inp[8], int32_t outp[8]) {
inline void naive_fwht_tuile(const int32_t* inp, int32_t* outp) {
    int32_t L1[8], L2[8];
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
inline void naive_fwht<8>(int32_t* inp) {
    int32_t part_1[8];
    for (int i = 0; i < 4; i++) {
        part_1[i]     = inp[i] + inp[i + 4];
        part_1[4 + i] = inp[i] - inp[i + 4];
    }
    naive_fwht_tuile(part_1, inp);
}
//
//
//
template <>
inline void naive_fwht<8>(int32_t * dst, const int32_t * src) {
    // int32_t part_1[8];
    for (int i = 0; i < 4; i++) {
        dst[i]     = src[i] + src[i + 4];
        dst[4 + i] = src[i] - src[i + 4];
    }
    naive_fwht_tuile(dst, dst);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<16>(int32_t* inp) {
    int32_t part_1[8];
    int32_t part_2[8];

    for (int i = 0; i < 8; i++)
        part_1[i] = inp[i] + inp[i + 8];
    for (int i = 0; i < 8; i++)
        part_2[i] = inp[i] - inp[i + 8];

    naive_fwht_tuile(part_1, inp + 0);
    naive_fwht_tuile(part_2, inp + 8);
}
//
//
//
template <>
inline void naive_fwht<16>(int32_t * dst, const int32_t * src) {
    for (int i = 0; i < 8; i++) {
        dst[i]     = src[i] + src[i + 8];
        dst[8 + i] = src[i] - src[i + 8];
    }
    naive_fwht_tuile(dst, dst + 0);
    naive_fwht_tuile(dst + 8, dst + 8);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<32>(int32_t* inp) {
    int32_t part_1[16];
    int32_t part_2[16];

    for (int i = 0; i < 16; i++) {
        part_1[i] = inp[i] + inp[i + 16];
        part_2[i] = inp[i] - inp[i + 16];
    }

    naive_fwht<16>(part_1);
    naive_fwht<16>(part_2);

    for (int i = 0; i < 16; i++) {
        inp[i]      = part_1[i];
        inp[16 + i] = part_2[i];
    }
}
//
//
//
template <>
inline void naive_fwht<32>(int32_t * dst, const int32_t * src) {
    for (int i = 0; i < 16; i++) {
        dst[i]      = src[i] + src[i + 16];
        dst[16 + i] = src[i] - src[i + 16];
    }
    naive_fwht<16>(dst, dst);
    naive_fwht<16>(dst + 16, dst + 16);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<64>(int32_t* inp) {
    int32_t part_1[32];
    int32_t part_2[32];

    for (int i = 0; i < 32; i++) {
        part_1[i] = inp[i] + inp[i + 32];
        part_2[i] = inp[i] - inp[i + 32];
    }

    naive_fwht<32>(part_1);
    naive_fwht<32>(part_2);

    for (int i = 0; i < 32; i++) {
        inp[i]      = part_1[i];
        inp[32 + i] = part_2[i];
    }
}
//
//
//
template <>
inline void naive_fwht<64>(int32_t* dst, const int32_t* src) {
    for (int i = 0; i < 32; i++) {
        dst[i]      = src[i] + src[i + 32];
        dst[32 + i] = src[i] - src[i + 32];
    }
    naive_fwht<32>(dst, dst);
    naive_fwht<32>(dst + 32, dst + 32);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<128>(int32_t* inp) {
    int32_t part_1[64], part_2[64];

    for (int i = 0; i < 64; i++) {
        part_1[i] = inp[i] + inp[i + 64];
        part_2[i] = inp[i] - inp[i + 64];
    }

    naive_fwht<64>(part_1);
    naive_fwht<64>(part_2);

    for (int i = 0; i < 64; i++) {
        inp[i + 0]  = part_1[i];
        inp[i + 64] = part_2[i];
    }
}
//
//
//
template <>
inline void naive_fwht<128>(int32_t * dst, const int32_t * src) {
    for (int i = 0; i < 64; i++) {
        dst[i]      = src[i] + src[i + 64];
        dst[64 + i] = src[i] - src[i + 64];
    }
    naive_fwht<64>(dst, dst);
    naive_fwht<64>(dst + 64, dst + 64);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<256>(int32_t* inp) {
    int32_t part_1[128];
    int32_t part_2[128];

    for (int i = 0; i < 128; i++) {
        part_1[i] = inp[i] + inp[i + 128];
        part_2[i] = inp[i] - inp[i + 128];
    }

    naive_fwht<128>(part_1);
    naive_fwht<128>(part_2);

    for (int i = 0; i < 128; i++) {
        inp[i + 0]   = part_1[i];
        inp[i + 128] = part_2[i];
    }
}
//
//
//
template <>
inline void naive_fwht<256>(int32_t* dst, const int32_t* src) {
    for (int i = 0; i < 128; i++) {
        dst[i]       = src[i] + src[i + 128];
        dst[128 + i] = src[i] - src[i + 128];
    }
    naive_fwht<128>(dst, dst);
    naive_fwht<128>(dst + 128, dst + 128);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<512>(int32_t* inp) {
    int32_t part_1[256];
    int32_t part_2[256];

    for (int i = 0; i < 256; i++) {
        part_1[i] = inp[i] + inp[i + 256];
        part_2[i] = inp[i] - inp[i + 256];
    }

    naive_fwht<256>(part_1);
    naive_fwht<256>(part_2);

    for (int i = 0; i < 256; i++) {
        inp[i +   0] = part_1[i];
        inp[i + 256] = part_2[i];
    }
}
//
//
//
template <>
inline void naive_fwht<512>(int32_t* dst, const int32_t* src) {
    for (int i = 0; i < 256; i++) {
        dst[i]       = src[i] + src[i + 256];
        dst[256 + i] = src[i] - src[i + 256];
    }
    naive_fwht<256>(dst, dst);
    naive_fwht<256>(dst + 256, dst + 256);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void naive_fwht<1024>(int32_t* inp) {
    int32_t part_1[512];
    int32_t part_2[512];

    for (int i = 0; i < 512; i++) {
        part_1[i] = inp[i] + inp[i + 512];
        part_2[i] = inp[i] - inp[i + 512];
    }

    naive_fwht<512>(part_1);
    naive_fwht<512>(part_2);

    for (int i = 0; i < 512; i++) {
        inp[i +   0] = part_1[i];
        inp[i + 512] = part_2[i];
    }
}
//
//
//
template <>
inline void naive_fwht<1024>(int32_t* dst, const int32_t* src) {
    for (int i = 0; i < 256; i++) {
        dst[i]       = src[i] + src[i + 256];
        dst[256 + i] = src[i] - src[i + 256];
    }
    naive_fwht<256>(dst, dst);
    naive_fwht<256>(dst + 256, dst + 256);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

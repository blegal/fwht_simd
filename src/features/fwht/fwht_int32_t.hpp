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
#include <cstdint>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include <arm_neon.h>

const uint32x4_t m0 = {0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF};
const uint32x4_t m1 = {0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF};

inline void fwht_int32_t_tuile_8(const int32x4_t X0, const int32x4_t X1, int32_t* outp)
{
    //
    //////////////////////////////////////////////////////
    //
    const int32x4_t HH = vaddq_s32(X0, X1);
    const int32x4_t BB = vaddq_s32(X0, vnegq_s32(X1));
    //
    //////////////////////////////////////////////////////
    //
    const int32x4_t N0 = vextq_s32(HH, HH, 2);
    const int32x4_t n0 = vnegq_s32(HH);
    const int32x4_t N1 = vbslq_s32(m0, n0, HH);
    const int32x4_t N2 = vaddq_s32(N0, N1);
    const int32x4_t v0 = vnegq_s32(N2);
    const int32x4_t V0 = vbslq_s32(m1, v0, N2);
    const int32x4_t V1 = vrev64q_s32(N2);
    const int32x4_t V2 = vaddq_s32(V0, V1);
    vst1q_s32(outp, V2);
    //
    //////////////////////////////////////////////////////
    //
    const int32x4_t O0 = vextq_s32(BB, BB, 2);
    const int32x4_t o0 = vnegq_s32(BB);
    const int32x4_t O1 = vbslq_s32(m0, o0, BB);
    const int32x4_t O2 = vaddq_s32(O0, O1);
    const int32x4_t q0 = vnegq_s32(O2);
    const int32x4_t Q0 = vbslq_s32(m1, q0, O2);
    const int32x4_t Q1 = vrev64q_s32(O2);
    const int32x4_t Q2 = vaddq_s32(Q0, Q1);
    vst1q_s32(outp + 4, Q2);
}

inline void fwht_int32_t_tuile_8(const int32_t* inp, int32_t* outp)
{
    const int32x4_t X0 = vld1q_s32(inp);
    const int32x4_t X1 = vld1q_s32(inp + 4);
    fwht_int32_t_tuile_8(X0, X1, outp);
}

inline void fwht_int32_t_tuile_16(const int32_t* inp, int32_t* outp)
{
    const int32x4_t X0 = vld1q_s32(inp     );
    const int32x4_t X1 = vld1q_s32(inp +  4);
    const int32x4_t X2 = vld1q_s32(inp +  8);
    const int32x4_t X3 = vld1q_s32(inp + 12);

    const int32x4_t Y0 = X0 + X2;
    const int32x4_t Y1 = X1 + X3;
    const int32x4_t Y2 = X0 - X2;
    const int32x4_t Y3 = X1 - X3;

    fwht_int32_t_tuile_8(Y0, Y1, outp    );
    fwht_int32_t_tuile_8(Y2, Y3, outp + 8);
}

inline void fwht_int32_t_tuile_32(const int32_t* inp, int32_t* outp)
{
    const int32x4_t X0 = vld1q_s32(inp     );
    const int32x4_t X1 = vld1q_s32(inp +  4);
    const int32x4_t X2 = vld1q_s32(inp +  8);
    const int32x4_t X3 = vld1q_s32(inp + 12);

    const int32x4_t X4 = vld1q_s32(inp + 16);
    const int32x4_t X5 = vld1q_s32(inp + 20);
    const int32x4_t X6 = vld1q_s32(inp + 24);
    const int32x4_t X7 = vld1q_s32(inp + 28);

    const int32x4_t Y0 = X0 + X4;
    const int32x4_t Y1 = X1 + X5;
    const int32x4_t Y2 = X2 + X6;
    const int32x4_t Y3 = X3 + X7;

    const int32x4_t Z0 = Y0 + Y2;
    const int32x4_t Z1 = Y1 + Y3;

    fwht_int32_t_tuile_8(Z0, Z1, outp );

    const int32x4_t Z2 = Y0 - Y2;
    const int32x4_t Z3 = Y1 - Y3;

    fwht_int32_t_tuile_8(Z2, Z3, outp + 8);

    const int32x4_t Y4 = X0 - X4;
    const int32x4_t Y5 = X1 - X5;
    const int32x4_t Y6 = X2 - X6;
    const int32x4_t Y7 = X3 - X7;

    const int32x4_t Z4 = Y4 + Y6;
    const int32x4_t Z5 = Y5 + Y7;

    fwht_int32_t_tuile_8(Z4, Z5, outp + 16);

    const int32x4_t Z6 = Y4 - Y6;
    const int32x4_t Z7 = Y5 - Y7;

    fwht_int32_t_tuile_8(Z6, Z7, outp + 24);

}

#else
inline void fwht_int32_t_tuile_8(const int32_t* inp, int32_t* outp) {
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
#endif
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <uint16_t galois_size>
inline void fwht(const int32_t* src, int32_t* dst) {
    constexpr int half_gf = galois_size / 2;
    static_assert(galois_size > 1, "galois_size must be over 1.");
    static_assert((half_gf << 1) == galois_size, "galois_size must be a power of 2.");
    int32_t part_1[half_gf];
    int32_t part_2[half_gf];
    for (int i = 0; i < half_gf; i++) {
        part_1[i] = src[i] + src[i + half_gf];
        part_2[i] = src[i] - src[i + half_gf];
    }
    if ( half_gf == 32 ) {
        fwht_int32_t_tuile_32(part_1, dst);
        fwht_int32_t_tuile_32(part_2, dst + half_gf);
    }else if ( half_gf == 16 ) {
        fwht_int32_t_tuile_16(part_1, dst);
        fwht_int32_t_tuile_16(part_2, dst + half_gf);
    }else if ( half_gf == 8 ) {
        fwht_int32_t_tuile_8(part_1, dst);
        fwht_int32_t_tuile_8(part_2, dst + half_gf);
    }else {
        fwht<half_gf>(part_1, dst);
        fwht<half_gf>(part_2, dst + half_gf);
    }
}

template <uint16_t galois_size>
inline void fwht(int32_t* dst_src) {
    constexpr int half_gf = galois_size / 2;
    static_assert(galois_size > 1, "galois_size must be over 1.");
    static_assert((half_gf << 1) == galois_size, "galois_size must be a power of 2.");
    int32_t part_1[half_gf];
    int32_t part_2[half_gf];
    for (int i = 0; i < half_gf; i++) {
        part_1[i] = dst_src[i] + dst_src[i + half_gf];
        part_2[i] = dst_src[i] - dst_src[i + half_gf];
    }
    if ( half_gf == 32 ) {
        fwht_int32_t_tuile_32(part_1, dst_src);
        fwht_int32_t_tuile_32(part_2, dst_src + half_gf);
    }else if ( half_gf == 16 ) {
        fwht_int32_t_tuile_16(part_1, dst_src);
        fwht_int32_t_tuile_16(part_2, dst_src + half_gf);
    }else if ( half_gf == 8 ) {
        fwht_int32_t_tuile_8(part_1, dst_src);
        fwht_int32_t_tuile_8(part_2, dst_src + half_gf);
    }else {
        fwht<half_gf>(part_1, dst_src);
        fwht<half_gf>(part_2, dst_src + half_gf);
    }
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
template <>
inline void fwht<8>(int32_t* inp) {
    fwht_int32_t_tuile_8(inp, inp);
}
//
template <>
inline void fwht<8>(const int32_t* src, int32_t* dst) {
    fwht_int32_t_tuile_8(src, dst);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

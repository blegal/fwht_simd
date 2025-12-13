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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include <arm_neon.h>
inline void fwht_size_8(float * __restrict outp, const float * __restrict inp) {
    const float32x4_t inp1 = vld1q_f32(inp    );
    const float32x4_t inp2 = vld1q_f32(inp + 4);

    const uint32x4_t m0 = {0x00000000, 0x00000000, 0x80000000, 0x80000000};
    const uint32x4_t m1 = {0x00000000, 0x80000000, 0x00000000, 0x80000000};
    //
    //////////////////////////////////////////////////////
    //
    const float32x4_t HH = vaddq_f32(inp1, inp2);
    const float32x4_t N0 = vextq_f32(HH, HH, 2);
    const float32x4_t N1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(HH), m0));
    const float32x4_t N2 = vaddq_f32(N0, N1);
    const float32x4_t V0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(N2), m1));
    const float32x4_t V1 = vrev64q_f32(N2);
    const float32x4_t V2 = vaddq_f32(V0, V1);
    vst1q_f32( outp, V2 );
    //
    //////////////////////////////////////////////////////
    //
    const float32x4_t BB = vsubq_f32(inp1, inp2);
    const float32x4_t O0 = vextq_f32(BB, BB, 2);
    const float32x4_t O1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(BB), m0));
    const float32x4_t O2 = vaddq_f32(O0, O1);
    const float32x4_t Q0 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(O2), m1));
    const float32x4_t Q1 = vrev64q_f32(O2);
    const float32x4_t Q2 = vaddq_f32(Q0, Q1);
    vst1q_f32( outp + 4, Q2 );
}
#else
inline void fwht_size_8(float * __restrict outp, const float * __restrict inp) {
    float L1[8], L2[8];
    L1[0] = inp[0] + inp[4];
    L1[1] = inp[1] + inp[5];
    L1[2] = inp[2] + inp[6];
    L1[3] = inp[3] + inp[7];
    L1[4] = inp[0] - inp[4];
    L1[5] = inp[1] - inp[5];
    L1[6] = inp[2] - inp[6];
    L1[7] = inp[3] - inp[7];

    L2[0] = L1[0] + L1[2];
    L2[1] = L1[1] + L1[3];
    L2[2] = L1[0] - L1[2];
    L2[3] = L1[1] - L1[3];
    L2[4] = L1[4] + L1[6];
    L2[5] = L1[5] + L1[7];
    L2[6] = L1[4] - L1[6];
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

template <size_t galois_size>
inline void fwht_template_spec8(float * dst, const float * src) {
    static_assert(galois_size > 8, "galois_size must be over 8.");
    constexpr size_t half_gf = galois_size >> 1;
    static_assert((half_gf << 1) == galois_size, "galois_size must be a power of 2.");
    float intermediary[galois_size];
    for (size_t j = 0; j < half_gf; j++) {
        intermediary[j] = src[j] + src[j + half_gf];
        intermediary[j + half_gf] = src[j] - src[j + half_gf];
    }
    fwht_template_spec8<half_gf>(dst, intermediary);
    fwht_template_spec8<half_gf>(dst + half_gf, intermediary + half_gf);
}

template <>
inline void fwht_template_spec8<8>(float * dst, const float * src) {
    float part_1[8];
    for (int i = 0; i < 4; i++) {
        part_1[i]     = src[i] + src[i + 4];
        part_1[4 + i] = src[i] - src[i + 4];
    }
    fwht_size_8(dst, part_1);
}

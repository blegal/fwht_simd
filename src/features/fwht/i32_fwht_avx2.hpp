#pragma once
#ifdef __AVX2__

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <immintrin.h>

template <uint16_t GF>
inline void i32_fwht_avx2(int x[]) {
    assert(false && "Not implemented for this GF");
}

template <uint16_t GF>
inline void i32_fwht_avx2(int x[], int y[]) {
    assert(false && "Not implemented for this GF");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

inline __m256i i32_fwht8_avx2(const __m256i x) {
    // const __m256i M0 = _mm256_castsi256_ps(_mm256_setr_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000));
    // const __m256i N0 = _mm256_xor_ps(x, M0);
    const __m256i neg0 = _mm256_sub_epi32(_mm256_setzero_si256(), x);
    const __m256i N0   = _mm256_blend_epi32(x, neg0, 0xF0);

    // const __m256i N1 = _mm256_permute2f128_ps(x, x, 0x01);
    const __m256i N1 = _mm256_permute2x128_si256(x, x, 0x01);

    // const __m256i N2 = _mm256_add_ps(N0, N1);
    const __m256i N2 = _mm256_add_epi32(N0, N1);

    // const __m256i M1 = _mm256_castsi256_ps(_mm256_setr_epi32(0x00000000, 0x00000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x80000000, 0x80000000));
    // const __m256i O0 = _mm256_xor_ps(N2, M1);
    const __m256i neg1 = _mm256_sub_epi32(_mm256_setzero_si256(), N2);
    const __m256i O0   = _mm256_blend_epi32(N2, neg1, 0xCC);

    // const __m256i O1 = _mm256_shuffle_ps(N2, N2, 0x4E);
    const __m256i O1 = _mm256_shuffle_epi32(N2, 0x4E);

    // const __m256i O2 = _mm256_add_ps(O0, O1);
    const __m256i O2 = _mm256_add_epi32(O0, O1);

    // const __m256i M2 = _mm256_castsi256_ps(_mm256_setr_epi32(0x00000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x80000000));
    // const __m256i P0 = _mm256_xor_ps(O2, M2);
    const __m256i neg2 = _mm256_sub_epi32(_mm256_setzero_si256(), O2);
    const __m256i P0   = _mm256_blend_epi32(O2, neg2, 0xAA);

    // const __m256i P1 = _mm256_shuffle_ps(O2, O2, 0xB1);
    const __m256i P1 = _mm256_shuffle_epi32(O2, 0xB1);

    // const __m256i P2 = _mm256_add_ps(P0, P1);
    const __m256i P2 = _mm256_add_epi32(P0, P1);

    return P2;
}

inline void i32_fwht16_avx2(__m256i * B0, __m256i * B1) {
    const __m256i C0 = _mm256_add_epi32(*B0, *B1);
    const __m256i C1 = _mm256_sub_epi32(*B0, *B1);
    *B0              = i32_fwht8_avx2(C0);
    *B1              = i32_fwht8_avx2(C1);
}

inline void i32_fwht16_terminale_avx2(const __m256i X0, const __m256i X1, int y[]) {
    // const __m256i HAUT = _mm256_add_ps(X0, X1);
    const __m256i HAUT = _mm256_add_epi32(X0, X1);

    // const __m256i N0 = _mm256_xor_ps(HAUT, M0);
    const __m256i neg340 = _mm256_sub_epi32(_mm256_setzero_si256(), HAUT);
    const __m256i N0     = _mm256_blend_epi32(HAUT, neg340, 0xF0);

    // const __m256i N1 = _mm256_permute2f128_ps(HAUT, HAUT, 0x01);
    const __m256i N1 = _mm256_permute2x128_si256(HAUT, HAUT, 0x01);

    // const __m256i N2 = _mm256_add_ps(N0, N1);
    const __m256i N2 = _mm256_add_epi32(N0, N1);

    // const __m256i O0 = _mm256_xor_ps(N2, M1);
    const __m256i neg_lanes0 = _mm256_sub_epi32(_mm256_setzero_si256(), N2);
    const __m256i O0         = _mm256_blend_epi32(N2, neg_lanes0, 0xCC);

    // const __m256i O1 = _mm256_shuffle_ps(N2, N2, 0x4E);
    const __m256i O1 = _mm256_shuffle_epi32(N2, 0x4E);

    // const __m256i O2 = _mm256_add_ps(O0, O1);
    const __m256i O2 = _mm256_add_epi32(O0, O1);

    // const __m256i P0 = _mm256_xor_ps(O2, M2);
    const __m256i neg_lanes1 = _mm256_sub_epi32(_mm256_setzero_si256(), O2);
    const __m256i P0         = _mm256_blend_epi32(O2, neg_lanes1, 0xAA);

    // const __m256i P1 = _mm256_shuffle_ps(O2, O2, 0xB1);
    const __m256i P1 = _mm256_shuffle_epi32(O2, 0xB1);

    // const __m256i P2 = _mm256_add_ps(P0, P1);
    const __m256i P2 = _mm256_add_epi32(P0, P1);

    // Process last 8 elements (bottom path)

    // const __m256i BAS = _mm256_sub_ps(X0, X1);
    const __m256i BAS = _mm256_sub_epi32(X0, X1);

    // const __m256i B0 = _mm256_xor_ps(BAS, M0);
    const __m256i neg34 = _mm256_sub_epi32(_mm256_setzero_si256(), BAS);
    const __m256i B0    = _mm256_blend_epi32(BAS, neg34, 0xF0);

    // const __m256i B1 = _mm256_permute2f128_ps(BAS, BAS, 0x01);
    const __m256i B1 = _mm256_permute2x128_si256(BAS, BAS, 0x01);

    // const __m256i B2 = _mm256_add_ps(B0, B1);
    const __m256i B2 = _mm256_add_epi32(B0, B1);

    // const __m256i BO0 = _mm256_xor_ps(B2, M1);
    const __m256i neg_lanes = _mm256_sub_epi32(_mm256_setzero_si256(), B2);
    const __m256i BO0       = _mm256_blend_epi32(B2, neg_lanes, 0xCC);

    // const __m256i BO1 = _mm256_shuffle_ps(B2, B2, 0x4E);
    const __m256i BO1 = _mm256_shuffle_epi32(B2, 0x4E);

    // const __m256i BO2 = _mm256_add_ps(BO0, BO1);
    const __m256i BO2 = _mm256_add_epi32(BO0, BO1);

    // const __m256i BP0 = _mm256_xor_ps(BO2, M2);
    const __m256i neg_lanes2 = _mm256_sub_epi32(_mm256_setzero_si256(), BO2);
    const __m256i BP0        = _mm256_blend_epi32(BO2, neg_lanes2, 0xAA);

    // const __m256i BP1 = _mm256_shuffle_ps(BO2, BO2, 0xB1);
    const __m256i BP1 = _mm256_shuffle_epi32(BO2, 0xB1);

    // const __m256i BP2 = _mm256_add_ps(BP0, BP1);
    const __m256i BP2 = _mm256_add_epi32(BP0, BP1);

    // _mm256_storeu_ps(y + 0, P2);
    _mm256_storeu_si256((__m256i *) (y + 0), P2);

    // _mm256_storeu_ps(y + 8, BP2);
    _mm256_storeu_si256((__m256i *) (y + 8), BP2);
}

inline void i32_fwht32_avx2(__m256i * B0, __m256i * B1, __m256i * B2, __m256i * B3) {
    __m256i C0 = _mm256_add_epi32(*B0, *B2);
    __m256i C1 = _mm256_add_epi32(*B1, *B3);

    __m256i C2 = _mm256_sub_epi32(*B0, *B2);
    __m256i C3 = _mm256_sub_epi32(*B1, *B3);

    i32_fwht16_avx2(&C0, &C1);
    i32_fwht16_avx2(&C2, &C3);

    *B0 = C0;
    *B1 = C1;
    *B2 = C2;
    *B3 = C3;
}

template <>
inline void i32_fwht_avx2<8>(int x[]) {
    const __m256i C0 = _mm256_loadu_si256((__m256i *) (x));
    const __m256i D0 = i32_fwht8_avx2(C0);
    _mm256_storeu_si256((__m256i *) x, D0);
}
template <>
inline void i32_fwht_avx2<8>(int x[], int y[]) {
    const __m256i C0 = _mm256_loadu_si256((__m256i *) (x));
    const __m256i D0 = i32_fwht8_avx2(C0);
    _mm256_storeu_si256((__m256i *) y, D0);
}

inline void i32_fwht16_flat_avx2(int x[], int y[]) {
    const __m256i X0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1 = _mm256_loadu_si256((__m256i *) (x + 8));
    i32_fwht16_terminale_avx2(X0, X1, y);
}

inline void i32_fwht32_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, int y[]) {
    const __m256i A0 = _mm256_add_epi32(X0, X2);
    const __m256i A1 = _mm256_add_epi32(X1, X3);
    const __m256i B0 = _mm256_sub_epi32(X0, X2);
    const __m256i B1 = _mm256_sub_epi32(X1, X3);
    i32_fwht16_terminale_avx2(A0, A1, y + 0);
    i32_fwht16_terminale_avx2(B0, B1, y + 16);
}

inline void i32_fwht32_flat_avx2(int x[], int y[]) {
    const __m256i X1 = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X2 = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X3 = _mm256_loadu_si256((__m256i *) (x + 24));

    const __m256i m0 = _mm256_add_epi32(X0, X2);
    const __m256i m1 = _mm256_add_epi32(X1, X3);
    const __m256i M0 = _mm256_sub_epi32(X0, X2);
    const __m256i M1 = _mm256_sub_epi32(X1, X3);

    i32_fwht16_terminale_avx2(m0, m1, y + 0);
    i32_fwht16_terminale_avx2(M0, M1, y + 16);
}

inline void i32_fwht64_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, __m256i X4, __m256i X5, __m256i X6, __m256i X7, int y[]) {
    const __m256i A0 = _mm256_add_epi32(X0, X4);
    const __m256i A1 = _mm256_add_epi32(X1, X5);
    const __m256i A2 = _mm256_add_epi32(X2, X6);
    const __m256i A3 = _mm256_add_epi32(X3, X7);
    const __m256i B0 = _mm256_sub_epi32(X0, X4);
    const __m256i B1 = _mm256_sub_epi32(X1, X5);
    const __m256i B2 = _mm256_sub_epi32(X2, X6);
    const __m256i B3 = _mm256_sub_epi32(X3, X7);

    i32_fwht32_terminale_avx2(A0, A1, A2, A3, y + 0);
    i32_fwht32_terminale_avx2(B0, B1, B2, B3, y + 32);
}

inline void i32_fwht64_flat_avx2(int x[], int y[]) {
    const __m256i X0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1 = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X2 = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X3 = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X4 = _mm256_loadu_si256((__m256i *) (x + 32));
    const __m256i X5 = _mm256_loadu_si256((__m256i *) (x + 40));
    const __m256i X6 = _mm256_loadu_si256((__m256i *) (x + 48));
    const __m256i X7 = _mm256_loadu_si256((__m256i *) (x + 56));

    const __m256i A0 = _mm256_add_epi32(X0, X4);
    const __m256i A1 = _mm256_add_epi32(X1, X5);
    const __m256i A2 = _mm256_add_epi32(X2, X6);
    const __m256i A3 = _mm256_add_epi32(X3, X7);
    const __m256i B0 = _mm256_sub_epi32(X0, X4);
    const __m256i B1 = _mm256_sub_epi32(X1, X5);
    const __m256i B2 = _mm256_sub_epi32(X2, X6);
    const __m256i B3 = _mm256_sub_epi32(X3, X7);

    i32_fwht32_terminale_avx2(A0, A1, A2, A3, y + 0);
    i32_fwht32_terminale_avx2(B0, B1, B2, B3, y + 32);
}

inline void i32_fwht128_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, __m256i X4, __m256i X5, __m256i X6, __m256i X7, __m256i X8, __m256i X9, __m256i X10, __m256i X11, __m256i X12, __m256i X13, __m256i X14, __m256i X15, int y[]) {
    const __m256i A0 = _mm256_add_epi32(X0, X8);
    const __m256i A1 = _mm256_add_epi32(X1, X9);
    const __m256i A2 = _mm256_add_epi32(X2, X10);
    const __m256i A3 = _mm256_add_epi32(X3, X11);
    const __m256i A4 = _mm256_add_epi32(X4, X12);
    const __m256i A5 = _mm256_add_epi32(X5, X13);
    const __m256i A6 = _mm256_add_epi32(X6, X14);
    const __m256i A7 = _mm256_add_epi32(X7, X15);

    const __m256i B0 = _mm256_sub_epi32(X0, X8);
    const __m256i B1 = _mm256_sub_epi32(X1, X9);
    const __m256i B2 = _mm256_sub_epi32(X2, X10);
    const __m256i B3 = _mm256_sub_epi32(X3, X11);
    const __m256i B4 = _mm256_sub_epi32(X4, X12);
    const __m256i B5 = _mm256_sub_epi32(X5, X13);
    const __m256i B6 = _mm256_sub_epi32(X6, X14);
    const __m256i B7 = _mm256_sub_epi32(X7, X15);

    i32_fwht64_terminale_avx2(A0, A1, A2, A3, A4, A5, A6, A7, y + 0);
    i32_fwht64_terminale_avx2(B0, B1, B2, B3, B4, B5, B6, B7, y + 64);
}

inline void i32_fwht128_flat_avx2(int x[], int y[]) {
    const __m256i X0  = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1  = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X2  = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X3  = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X4  = _mm256_loadu_si256((__m256i *) (x + 32));
    const __m256i X5  = _mm256_loadu_si256((__m256i *) (x + 40));
    const __m256i X6  = _mm256_loadu_si256((__m256i *) (x + 48));
    const __m256i X7  = _mm256_loadu_si256((__m256i *) (x + 56));
    const __m256i X8  = _mm256_loadu_si256((__m256i *) (x + 64));
    const __m256i X9  = _mm256_loadu_si256((__m256i *) (x + 72));
    const __m256i X10 = _mm256_loadu_si256((__m256i *) (x + 80));
    const __m256i X11 = _mm256_loadu_si256((__m256i *) (x + 88));
    const __m256i X12 = _mm256_loadu_si256((__m256i *) (x + 96));
    const __m256i X13 = _mm256_loadu_si256((__m256i *) (x + 104));
    const __m256i X14 = _mm256_loadu_si256((__m256i *) (x + 112));
    const __m256i X15 = _mm256_loadu_si256((__m256i *) (x + 120));

    i32_fwht128_terminale_avx2(X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, y);
}

// For 256 - needs implementation of i32_fwht256_flat_avx2
inline void i32_fwht256_flat_avx2(int x[], int y[]) {
    const __m256i X0  = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1  = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X2  = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X3  = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X4  = _mm256_loadu_si256((__m256i *) (x + 32));
    const __m256i X5  = _mm256_loadu_si256((__m256i *) (x + 40));
    const __m256i X6  = _mm256_loadu_si256((__m256i *) (x + 48));
    const __m256i X7  = _mm256_loadu_si256((__m256i *) (x + 56));
    const __m256i X8  = _mm256_loadu_si256((__m256i *) (x + 64));
    const __m256i X9  = _mm256_loadu_si256((__m256i *) (x + 72));
    const __m256i X10 = _mm256_loadu_si256((__m256i *) (x + 80));
    const __m256i X11 = _mm256_loadu_si256((__m256i *) (x + 88));
    const __m256i X12 = _mm256_loadu_si256((__m256i *) (x + 96));
    const __m256i X13 = _mm256_loadu_si256((__m256i *) (x + 104));
    const __m256i X14 = _mm256_loadu_si256((__m256i *) (x + 112));
    const __m256i X15 = _mm256_loadu_si256((__m256i *) (x + 120));

    const __m256i Y0  = _mm256_loadu_si256((__m256i *) (x + 128));
    const __m256i Y1  = _mm256_loadu_si256((__m256i *) (x + 136));
    const __m256i Y2  = _mm256_loadu_si256((__m256i *) (x + 144));
    const __m256i Y3  = _mm256_loadu_si256((__m256i *) (x + 152));
    const __m256i Y4  = _mm256_loadu_si256((__m256i *) (x + 160));
    const __m256i Y5  = _mm256_loadu_si256((__m256i *) (x + 168));
    const __m256i Y6  = _mm256_loadu_si256((__m256i *) (x + 176));
    const __m256i Y7  = _mm256_loadu_si256((__m256i *) (x + 184));
    const __m256i Y8  = _mm256_loadu_si256((__m256i *) (x + 192));
    const __m256i Y9  = _mm256_loadu_si256((__m256i *) (x + 200));
    const __m256i Y10 = _mm256_loadu_si256((__m256i *) (x + 208));
    const __m256i Y11 = _mm256_loadu_si256((__m256i *) (x + 216));
    const __m256i Y12 = _mm256_loadu_si256((__m256i *) (x + 224));
    const __m256i Y13 = _mm256_loadu_si256((__m256i *) (x + 232));
    const __m256i Y14 = _mm256_loadu_si256((__m256i *) (x + 240));
    const __m256i Y15 = _mm256_loadu_si256((__m256i *) (x + 248));

    // Top butterfly: X + Y
    const __m256i A0  = _mm256_add_epi32(X0, Y0);
    const __m256i A1  = _mm256_add_epi32(X1, Y1);
    const __m256i A2  = _mm256_add_epi32(X2, Y2);
    const __m256i A3  = _mm256_add_epi32(X3, Y3);
    const __m256i A4  = _mm256_add_epi32(X4, Y4);
    const __m256i A5  = _mm256_add_epi32(X5, Y5);
    const __m256i A6  = _mm256_add_epi32(X6, Y6);
    const __m256i A7  = _mm256_add_epi32(X7, Y7);
    const __m256i A8  = _mm256_add_epi32(X8, Y8);
    const __m256i A9  = _mm256_add_epi32(X9, Y9);
    const __m256i A10 = _mm256_add_epi32(X10, Y10);
    const __m256i A11 = _mm256_add_epi32(X11, Y11);
    const __m256i A12 = _mm256_add_epi32(X12, Y12);
    const __m256i A13 = _mm256_add_epi32(X13, Y13);
    const __m256i A14 = _mm256_add_epi32(X14, Y14);
    const __m256i A15 = _mm256_add_epi32(X15, Y15);

    // Bottom butterfly: X - Y
    const __m256i B0  = _mm256_sub_epi32(X0, Y0);
    const __m256i B1  = _mm256_sub_epi32(X1, Y1);
    const __m256i B2  = _mm256_sub_epi32(X2, Y2);
    const __m256i B3  = _mm256_sub_epi32(X3, Y3);
    const __m256i B4  = _mm256_sub_epi32(X4, Y4);
    const __m256i B5  = _mm256_sub_epi32(X5, Y5);
    const __m256i B6  = _mm256_sub_epi32(X6, Y6);
    const __m256i B7  = _mm256_sub_epi32(X7, Y7);
    const __m256i B8  = _mm256_sub_epi32(X8, Y8);
    const __m256i B9  = _mm256_sub_epi32(X9, Y9);
    const __m256i B10 = _mm256_sub_epi32(X10, Y10);
    const __m256i B11 = _mm256_sub_epi32(X11, Y11);
    const __m256i B12 = _mm256_sub_epi32(X12, Y12);
    const __m256i B13 = _mm256_sub_epi32(X13, Y13);
    const __m256i B14 = _mm256_sub_epi32(X14, Y14);
    const __m256i B15 = _mm256_sub_epi32(X15, Y15);

    i32_fwht128_terminale_avx2(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, y);
    i32_fwht128_terminale_avx2(B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15, y + 128);
}

template <>
inline void i32_fwht_avx2<256>(int x[]) {
    i32_fwht256_flat_avx2(x, x);
}
template <>
inline void i32_fwht_avx2<256>(int x[], int y[]) {
    i32_fwht256_flat_avx2(x, y);
}

// For 512
template <>
inline void i32_fwht_avx2<512>(int x[]) {
    const int simd = sizeof(__m256i) / sizeof(int);
    for (int i = 0; i < 256; i += simd) {
        const __m256i A = _mm256_loadu_si256((__m256i *) (x + i));
        const __m256i B = _mm256_loadu_si256((__m256i *) (x + i + 256));
        const __m256i C = _mm256_add_epi32(A, B);
        const __m256i D = _mm256_sub_epi32(A, B);
        _mm256_storeu_si256((__m256i *) (x + i + 0), C);
        _mm256_storeu_si256((__m256i *) (x + i + 256), D);
    }
    i32_fwht_avx2<256>(x + 0);
    i32_fwht_avx2<256>(x + 256);
}
template <>
inline void i32_fwht_avx2<512>(int x[], int y[]) {
    const int simd = sizeof(__m256i) / sizeof(int);
    for (int i = 0; i < 256; i += simd) {
        const __m256i A = _mm256_loadu_si256((__m256i *) (x + i));
        const __m256i B = _mm256_loadu_si256((__m256i *) (x + i + 256));
        const __m256i C = _mm256_add_epi32(A, B);
        const __m256i D = _mm256_sub_epi32(A, B);
        _mm256_storeu_si256((__m256i *) (y + i + 0), C);
        _mm256_storeu_si256((__m256i *) (y + i + 256), D);
    }
    i32_fwht_avx2<256>(y + 0);
    i32_fwht_avx2<256>(y + 256);
}

template <>
inline void i32_fwht_avx2<16>(int x[]) {
    i32_fwht16_flat_avx2(x, x);
}

template <>
inline void i32_fwht_avx2<16>(int x[], int y[]) {
    i32_fwht16_flat_avx2(x, y);
}

template <>
inline void i32_fwht_avx2<32>(int x[]) {
    i32_fwht32_flat_avx2(x, x);
}
template <>
inline void i32_fwht_avx2<32>(int x[], int y[]) {
    i32_fwht32_flat_avx2(x, y);
}

template <>
inline void i32_fwht_avx2<64>(int x[]) {
    i32_fwht64_flat_avx2(x, x);
}
template <>
inline void i32_fwht_avx2<64>(int x[], int y[]) {
    i32_fwht64_flat_avx2(x, y);
}

template <>
inline void i32_fwht_avx2<128>(int x[]) {
    i32_fwht128_flat_avx2(x, x);
}
template <>
inline void i32_fwht_avx2<128>(int x[], int y[]) {
    i32_fwht128_flat_avx2(x, y);
}

// For 1024
template <>
inline void i32_fwht_avx2<1024>(int x[]) {
    const int simd = sizeof(__m256i) / sizeof(int);
    for (int i = 0; i < 512; i += simd) {
        const __m256i A = _mm256_loadu_si256((__m256i *) (x + i));
        const __m256i B = _mm256_loadu_si256((__m256i *) (x + i + 512));
        const __m256i C = _mm256_add_epi32(A, B);
        const __m256i D = _mm256_sub_epi32(A, B);
        _mm256_storeu_si256((__m256i *) (x + i + 0), C);
        _mm256_storeu_si256((__m256i *) (x + i + 512), D);
    }
    i32_fwht_avx2<512>(x);
    i32_fwht_avx2<512>(x + 512);
}
template <>
inline void i32_fwht_avx2<1024>(int x[], int y[]) {
    const int simd = sizeof(__m256i) / sizeof(int);
    for (int i = 0; i < 512; i += simd) {
        const __m256i A = _mm256_loadu_si256((__m256i *) (x + i));
        const __m256i B = _mm256_loadu_si256((__m256i *) (x + i + 512));
        const __m256i C = _mm256_add_epi32(A, B);
        const __m256i D = _mm256_sub_epi32(A, B);
        _mm256_storeu_si256((__m256i *) (y + i + 0), C);
        _mm256_storeu_si256((__m256i *) (y + i + 512), D);
    }
    i32_fwht_avx2<512>(y + 0);
    i32_fwht_avx2<512>(y + 512);
}

#endif // __AVX2__
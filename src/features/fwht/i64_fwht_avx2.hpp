
#ifndef FWHT8_AVX2_HPP
#define FWHT8_AVX2_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <immintrin.h>

template <uint16_t GF>
inline void i64_fwht_avx2(int64_t x[]) {
    assert(x != 0);
    assert(true);
    exit(x != nullptr);
}

template <uint16_t GF>
inline void i64_fwht_avx2(int64_t x[], int64_t y[]) {
    assert(x != 0);
    assert(y != 0);
    exit(x != NULL && y != NULL);
}

inline void i64_fwht8_terminale_avx2(const __m256i X0, const __m256i X1, int64_t y[]) {

    const __m256i HAUT   = _mm256_add_epi64(X0, X1);
    const __m256i neg340 = _mm256_sub_epi64(_mm256_setzero_si256(), HAUT);
    const __m256i N0     = _mm256_blend_epi32(HAUT, neg340, 0xF0);

    const __m256i N1         = _mm256_permute2x128_si256(HAUT, HAUT, 0x01);
    const __m256i N2         = _mm256_add_epi64(N0, N1);
    const __m256i neg_lanes0 = _mm256_sub_epi64(_mm256_setzero_si256(), N2);
    const __m256i O0         = _mm256_blend_epi32(N2, neg_lanes0, 0xcc);

    const __m256i O1 = _mm256_permute4x64_epi64(N2, 0xB1);
    const __m256i P2 = _mm256_add_epi64(O0, O1);
    _mm256_storeu_si256((__m256i *) (y + 0), P2);

    const __m256i BAS       = _mm256_sub_epi64(X0, X1);
    const __m256i neg34     = _mm256_sub_epi64(_mm256_setzero_si256(), BAS);
    const __m256i B0        = _mm256_blend_epi32(BAS, neg34, 0xF0);
    const __m256i B1        = _mm256_permute2x128_si256(BAS, BAS, 0x01);
    const __m256i B2        = _mm256_add_epi64(B0, B1);
    const __m256i neg_lanes = _mm256_sub_epi64(_mm256_setzero_si256(), B2);
    const __m256i BO0       = _mm256_blend_epi32(B2, neg_lanes, 0xCC);
    const __m256i BO1       = _mm256_permute4x64_epi64(B2, 0xB1);
    const __m256i BO2       = _mm256_add_epi64(BO0, BO1);
    _mm256_storeu_si256((__m256i *) (y + 4), BO2);
}

inline void i64_fwht8_flat_avx2(int64_t x[], int64_t y[]) {
    const __m256i X0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1 = _mm256_loadu_si256((__m256i *) (x + 4));
    i64_fwht8_terminale_avx2(X0, X1, y);
}

inline void i64_fwht16_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, int64_t y[]) {
    const __m256i A0 = X0 + X2, A1 = X1 + X3;
    const __m256i B0 = X0 - X2, B1 = X1 - X3;
    i64_fwht8_terminale_avx2(A0, A1, y + 0);
    i64_fwht8_terminale_avx2(B0, B1, y + 8);
}

inline void i64_fwht16_flat_avx2(int64_t x[], int64_t y[]) {
    const __m256i X0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1 = _mm256_loadu_si256((__m256i *) (x + 4));
    const __m256i X2 = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X3 = _mm256_loadu_si256((__m256i *) (x + 12));

    const __m256i m0 = X0 + X2, m1 = X1 + X3;
    const __m256i M0 = X0 - X2, M1 = X1 - X3;

    i64_fwht8_terminale_avx2(m0, m1, y + 0);
    i64_fwht8_terminale_avx2(M0, M1, y + 8);
}

inline void i64_fwht32_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, __m256i X4, __m256i X5, __m256i X6, __m256i X7, int64_t y[]) {
    const __m256i A0 = X0 + X4, A1 = X1 + X5;
    const __m256i A2 = X2 + X6, A3 = X3 + X7;
    const __m256i B0 = X0 - X4, B1 = X1 - X5;
    const __m256i B2 = X2 - X6, B3 = X3 - X7;
    i64_fwht16_terminale_avx2(A0, A1, A2, A3, y + 0);
    i64_fwht16_terminale_avx2(B0, B1, B2, B3, y + 16);
}

inline void i64_fwht32_flat_avx2(int64_t x[], int64_t y[]) {
    const __m256i X0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1 = _mm256_loadu_si256((__m256i *) (x + 4));
    const __m256i X2 = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X3 = _mm256_loadu_si256((__m256i *) (x + 12));
    const __m256i X4 = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X5 = _mm256_loadu_si256((__m256i *) (x + 20));
    const __m256i X6 = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X7 = _mm256_loadu_si256((__m256i *) (x + 28));

    const __m256i m0 = X0 + X4, m1 = X1 + X5;
    const __m256i M0 = X0 - X4, M1 = X1 - X5;

    i64_fwht16_terminale_avx2(m0, m1, X2 + X6, X3 + X7, y + 0);
    i64_fwht16_terminale_avx2(M0, M1, X2 - X6, X3 - X7, y + 16);
}

inline void i64_fwht64_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, __m256i X4, __m256i X5, __m256i X6, __m256i X7, __m256i X8, __m256i X9, __m256i X10, __m256i X11, __m256i X12, __m256i X13, __m256i X14, __m256i X15, int64_t y[]) {
    const __m256i A0 = X0 + X8, A1 = X1 + X9;
    const __m256i A2 = X2 + X10, A3 = X3 + X11;
    const __m256i A4 = X4 + X12, A5 = X5 + X13;
    const __m256i A6 = X6 + X14, A7 = X7 + X15;
    const __m256i B0 = X0 - X8, B1 = X1 - X9;
    const __m256i B2 = X2 - X10, B3 = X3 - X11;
    const __m256i B4 = X4 - X12, B5 = X5 - X13;
    const __m256i B6 = X6 - X14, B7 = X7 - X15;
    i64_fwht32_terminale_avx2(A0, A1, A2, A3, A4, A5, A6, A7, y + 0);
    i64_fwht32_terminale_avx2(B0, B1, B2, B3, B4, B5, B6, B7, y + 32);
}

inline void i64_fwht64_flat_avx2(int64_t x[], int64_t y[]) {
    const __m256i X0  = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1  = _mm256_loadu_si256((__m256i *) (x + 4));
    const __m256i X2  = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X3  = _mm256_loadu_si256((__m256i *) (x + 12));
    const __m256i X4  = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X5  = _mm256_loadu_si256((__m256i *) (x + 20));
    const __m256i X6  = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X7  = _mm256_loadu_si256((__m256i *) (x + 28));
    const __m256i X8  = _mm256_loadu_si256((__m256i *) (x + 32));
    const __m256i X9  = _mm256_loadu_si256((__m256i *) (x + 36));
    const __m256i X10 = _mm256_loadu_si256((__m256i *) (x + 40));
    const __m256i X11 = _mm256_loadu_si256((__m256i *) (x + 44));
    const __m256i X12 = _mm256_loadu_si256((__m256i *) (x + 48));
    const __m256i X13 = _mm256_loadu_si256((__m256i *) (x + 52));
    const __m256i X14 = _mm256_loadu_si256((__m256i *) (x + 56));
    const __m256i X15 = _mm256_loadu_si256((__m256i *) (x + 60));

    const __m256i m0 = X0 + X4, m1 = X1 + X5;
    const __m256i M0 = X0 - X4, M1 = X1 - X5;

    i64_fwht32_terminale_avx2(m0, m1, X2 + X6, X3 + X7, X8 + X12, X9 + X13, X10 + X14, X11 + X15, y + 0);
    i64_fwht32_terminale_avx2(M0, M1, X2 - X6, X3 - X7, X8 - X12, X9 - X13, X10 - X14, X11 - X15, y + 32);
}

inline void i64_fwht128_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, __m256i X4, __m256i X5, __m256i X6, __m256i X7, __m256i X8, __m256i X9, __m256i X10, __m256i X11, __m256i X12, __m256i X13, __m256i X14, __m256i X15, __m256i X16, __m256i X17, __m256i X18, __m256i X19, __m256i X20, __m256i X21, __m256i X22, __m256i X23, __m256i X24, __m256i X25, __m256i X26, __m256i X27, __m256i X28, __m256i X29, __m256i X30, __m256i X31, int64_t y[]) {
    const __m256i A0 = X0 + X16, A1 = X1 + X17;
    const __m256i A2 = X2 + X18, A3 = X3 + X19;
    const __m256i A4 = X4 + X20, A5 = X5 + X21;
    const __m256i A6 = X6 + X22, A7 = X7 + X23;
    const __m256i A8 = X8 + X24, A9 = X9 + X25;
    const __m256i A10 = X10 + X26, A11 = X11 + X27;
    const __m256i A12 = X12 + X28, A13 = X13 + X29;
    const __m256i A14 = X14 + X30, A15 = X15 + X31;
    const __m256i B0 = X0 - X16, B1 = X1 - X17;
    const __m256i B2 = X2 - X18, B3 = X3 - X19;
    const __m256i B4 = X4 - X20, B5 = X5 - X21;
    const __m256i B6 = X6 - X22, B7 = X7 - X23;
    const __m256i B8 = X8 - X24, B9 = X9 - X25;
    const __m256i B10 = X10 - X26, B11 = X11 - X27;
    const __m256i B12 = X12 - X28, B13 = X13 - X29;
    const __m256i B14 = X14 - X30, B15 = X15 - X31;
    i64_fwht64_terminale_avx2(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, y + 0);
    i64_fwht64_terminale_avx2(B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15, y + 64);
}

inline void i64_fwht128_flat_avx2(int64_t x[], int64_t y[]) {
    const __m256i X0  = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1  = _mm256_loadu_si256((__m256i *) (x + 4));
    const __m256i X2  = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X3  = _mm256_loadu_si256((__m256i *) (x + 12));
    const __m256i X4  = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X5  = _mm256_loadu_si256((__m256i *) (x + 20));
    const __m256i X6  = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X7  = _mm256_loadu_si256((__m256i *) (x + 28));
    const __m256i X8  = _mm256_loadu_si256((__m256i *) (x + 32));
    const __m256i X9  = _mm256_loadu_si256((__m256i *) (x + 36));
    const __m256i X10 = _mm256_loadu_si256((__m256i *) (x + 40));
    const __m256i X11 = _mm256_loadu_si256((__m256i *) (x + 44));
    const __m256i X12 = _mm256_loadu_si256((__m256i *) (x + 48));
    const __m256i X13 = _mm256_loadu_si256((__m256i *) (x + 52));
    const __m256i X14 = _mm256_loadu_si256((__m256i *) (x + 56));
    const __m256i X15 = _mm256_loadu_si256((__m256i *) (x + 60));
    const __m256i X16 = _mm256_loadu_si256((__m256i *) (x + 64));
    const __m256i X17 = _mm256_loadu_si256((__m256i *) (x + 68));
    const __m256i X18 = _mm256_loadu_si256((__m256i *) (x + 72));
    const __m256i X19 = _mm256_loadu_si256((__m256i *) (x + 76));
    const __m256i X20 = _mm256_loadu_si256((__m256i *) (x + 80));
    const __m256i X21 = _mm256_loadu_si256((__m256i *) (x + 84));
    const __m256i X22 = _mm256_loadu_si256((__m256i *) (x + 88));
    const __m256i X23 = _mm256_loadu_si256((__m256i *) (x + 92));
    const __m256i X24 = _mm256_loadu_si256((__m256i *) (x + 96));
    const __m256i X25 = _mm256_loadu_si256((__m256i *) (x + 100));
    const __m256i X26 = _mm256_loadu_si256((__m256i *) (x + 104));
    const __m256i X27 = _mm256_loadu_si256((__m256i *) (x + 108));
    const __m256i X28 = _mm256_loadu_si256((__m256i *) (x + 112));
    const __m256i X29 = _mm256_loadu_si256((__m256i *) (x + 116));
    const __m256i X30 = _mm256_loadu_si256((__m256i *) (x + 120));
    const __m256i X31 = _mm256_loadu_si256((__m256i *) (x + 124));

    const __m256i m0 = X0 + X16, m1 = X1 + X17;
    const __m256i M0 = X0 - X16, M1 = X1 - X17;

    i64_fwht64_terminale_avx2(m0, m1, X2 + X18, X3 + X19, X4 + X20, X5 + X21, X6 + X22, X7 + X23, X8 + X24, X9 + X25, X10 + X26, X11 + X27, X12 + X28, X13 + X29, X14 + X30, X15 + X31, y + 0);
    i64_fwht64_terminale_avx2(M0, M1, X2 - X18, X3 - X19, X4 - X20, X5 - X21, X6 - X22, X7 - X23, X8 - X24, X9 - X25, X10 - X26, X11 - X27, X12 - X28, X13 - X29, X14 - X30, X15 - X31, y + 64);
}

inline void i64_fwht256_terminale_avx2(__m256i X0, __m256i X1, __m256i X2, __m256i X3, __m256i X4, __m256i X5, __m256i X6, __m256i X7, __m256i X8, __m256i X9, __m256i X10, __m256i X11, __m256i X12, __m256i X13, __m256i X14, __m256i X15,
                                       __m256i X16, __m256i X17, __m256i X18, __m256i X19, __m256i X20, __m256i X21, __m256i X22, __m256i X23, __m256i X24, __m256i X25, __m256i X26, __m256i X27, __m256i X28, __m256i X29, __m256i X30, __m256i X31, __m256i X32, __m256i X33, __m256i X34, __m256i X35, __m256i X36, __m256i X37, __m256i X38, __m256i X39, __m256i X40, __m256i X41, __m256i X42, __m256i X43, __m256i X44, __m256i X45, __m256i X46, __m256i X47, __m256i X48, __m256i X49, __m256i X50, __m256i X51, __m256i X52, __m256i X53, __m256i X54, __m256i X55, __m256i X56, __m256i X57, __m256i X58, __m256i X59, __m256i X60, __m256i X61, __m256i X62, __m256i X63, int64_t y[]) {
    const __m256i A0 = X0 + X32, A1 = X1 + X33;
    const __m256i A2 = X2 + X34, A3 = X3 + X35;
    const __m256i A4 = X4 + X36, A5 = X5 + X37;
    const __m256i A6 = X6 + X38, A7 = X7 + X39;
    const __m256i A8 = X8 + X40, A9 = X9 + X41;
    const __m256i A10 = X10 + X42, A11 = X11 + X43;
    const __m256i A12 = X12 + X44, A13 = X13 + X45;
    const __m256i A14 = X14 + X46, A15 = X15 + X47;
    const __m256i A16 = X16 + X48, A17 = X17 + X49;
    const __m256i A18 = X18 + X50, A19 = X19 + X51;
    const __m256i A20 = X20 + X52, A21 = X21 + X53;
    const __m256i A22 = X22 + X54, A23 = X23 + X55;
    const __m256i A24 = X24 + X56, A25 = X25 + X57;
    const __m256i A26 = X26 + X58, A27 = X27 + X59;
    const __m256i A28 = X28 + X60, A29 = X29 + X61;
    const __m256i A30 = X30 + X62, A31 = X31 + X63;
    const __m256i B0 = X0 - X32, B1 = X1 - X33;
    const __m256i B2 = X2 - X34, B3 = X3 - X35;
    const __m256i B4 = X4 - X36, B5 = X5 - X37;
    const __m256i B6 = X6 - X38, B7 = X7 - X39;
    const __m256i B8 = X8 - X40, B9 = X9 - X41;
    const __m256i B10 = X10 - X42, B11 = X11 - X43;
    const __m256i B12 = X12 - X44, B13 = X13 - X45;
    const __m256i B14 = X14 - X46, B15 = X15 - X47;
    const __m256i B16 = X16 - X48, B17 = X17 - X49;
    const __m256i B18 = X18 - X50, B19 = X19 - X51;
    const __m256i B20 = X20 - X52, B21 = X21 - X53;
    const __m256i B22 = X22 - X54, B23 = X23 - X55;
    const __m256i B24 = X24 - X56, B25 = X25 - X57;
    const __m256i B26 = X26 - X58, B27 = X27 - X59;
    const __m256i B28 = X28 - X60, B29 = X29 - X61;
    const __m256i B30 = X30 - X62, B31 = X31 - X63;
    i64_fwht128_terminale_avx2(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
                               A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, y + 0);
    i64_fwht128_terminale_avx2(B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
                               B16, B17, B18, B19, B20, B21, B22, B23, B24, B25, B26, B27, B28, B29, B30, B31, y + 128);
}

inline void i64_fwht256_flat_avx2(int64_t x[], int64_t y[]) {
    const __m256i X0  = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i X1  = _mm256_loadu_si256((__m256i *) (x + 4));
    const __m256i X2  = _mm256_loadu_si256((__m256i *) (x + 8));
    const __m256i X3  = _mm256_loadu_si256((__m256i *) (x + 12));
    const __m256i X4  = _mm256_loadu_si256((__m256i *) (x + 16));
    const __m256i X5  = _mm256_loadu_si256((__m256i *) (x + 20));
    const __m256i X6  = _mm256_loadu_si256((__m256i *) (x + 24));
    const __m256i X7  = _mm256_loadu_si256((__m256i *) (x + 28));
    const __m256i X8  = _mm256_loadu_si256((__m256i *) (x + 32));
    const __m256i X9  = _mm256_loadu_si256((__m256i *) (x + 36));
    const __m256i X10 = _mm256_loadu_si256((__m256i *) (x + 40));
    const __m256i X11 = _mm256_loadu_si256((__m256i *) (x + 44));
    const __m256i X12 = _mm256_loadu_si256((__m256i *) (x + 48));
    const __m256i X13 = _mm256_loadu_si256((__m256i *) (x + 52));
    const __m256i X14 = _mm256_loadu_si256((__m256i *) (x + 56));
    const __m256i X15 = _mm256_loadu_si256((__m256i *) (x + 60));
    const __m256i X16 = _mm256_loadu_si256((__m256i *) (x + 64));
    const __m256i X17 = _mm256_loadu_si256((__m256i *) (x + 68));
    const __m256i X18 = _mm256_loadu_si256((__m256i *) (x + 72));
    const __m256i X19 = _mm256_loadu_si256((__m256i *) (x + 76));
    const __m256i X20 = _mm256_loadu_si256((__m256i *) (x + 80));
    const __m256i X21 = _mm256_loadu_si256((__m256i *) (x + 84));
    const __m256i X22 = _mm256_loadu_si256((__m256i *) (x + 88));
    const __m256i X23 = _mm256_loadu_si256((__m256i *) (x + 92));
    const __m256i X24 = _mm256_loadu_si256((__m256i *) (x + 96));
    const __m256i X25 = _mm256_loadu_si256((__m256i *) (x + 100));
    const __m256i X26 = _mm256_loadu_si256((__m256i *) (x + 104));
    const __m256i X27 = _mm256_loadu_si256((__m256i *) (x + 108));
    const __m256i X28 = _mm256_loadu_si256((__m256i *) (x + 112));
    const __m256i X29 = _mm256_loadu_si256((__m256i *) (x + 116));
    const __m256i X30 = _mm256_loadu_si256((__m256i *) (x + 120));
    const __m256i X31 = _mm256_loadu_si256((__m256i *) (x + 124));
    const __m256i X32 = _mm256_loadu_si256((__m256i *) (x + 128));
    const __m256i X33 = _mm256_loadu_si256((__m256i *) (x + 132));
    const __m256i X34 = _mm256_loadu_si256((__m256i *) (x + 136));
    const __m256i X35 = _mm256_loadu_si256((__m256i *) (x + 140));
    const __m256i X36 = _mm256_loadu_si256((__m256i *) (x + 144));
    const __m256i X37 = _mm256_loadu_si256((__m256i *) (x + 148));
    const __m256i X38 = _mm256_loadu_si256((__m256i *) (x + 152));
    const __m256i X39 = _mm256_loadu_si256((__m256i *) (x + 156));
    const __m256i X40 = _mm256_loadu_si256((__m256i *) (x + 160));
    const __m256i X41 = _mm256_loadu_si256((__m256i *) (x + 164));
    const __m256i X42 = _mm256_loadu_si256((__m256i *) (x + 168));
    const __m256i X43 = _mm256_loadu_si256((__m256i *) (x + 172));
    const __m256i X44 = _mm256_loadu_si256((__m256i *) (x + 176));
    const __m256i X45 = _mm256_loadu_si256((__m256i *) (x + 180));
    const __m256i X46 = _mm256_loadu_si256((__m256i *) (x + 184));
    const __m256i X47 = _mm256_loadu_si256((__m256i *) (x + 188));
    const __m256i X48 = _mm256_loadu_si256((__m256i *) (x + 192));
    const __m256i X49 = _mm256_loadu_si256((__m256i *) (x + 196));
    const __m256i X50 = _mm256_loadu_si256((__m256i *) (x + 200));
    const __m256i X51 = _mm256_loadu_si256((__m256i *) (x + 204));
    const __m256i X52 = _mm256_loadu_si256((__m256i *) (x + 208));
    const __m256i X53 = _mm256_loadu_si256((__m256i *) (x + 212));
    const __m256i X54 = _mm256_loadu_si256((__m256i *) (x + 216));
    const __m256i X55 = _mm256_loadu_si256((__m256i *) (x + 220));
    const __m256i X56 = _mm256_loadu_si256((__m256i *) (x + 224));
    const __m256i X57 = _mm256_loadu_si256((__m256i *) (x + 228));
    const __m256i X58 = _mm256_loadu_si256((__m256i *) (x + 232));
    const __m256i X59 = _mm256_loadu_si256((__m256i *) (x + 236));
    const __m256i X60 = _mm256_loadu_si256((__m256i *) (x + 240));
    const __m256i X61 = _mm256_loadu_si256((__m256i *) (x + 244));
    const __m256i X62 = _mm256_loadu_si256((__m256i *) (x + 248));
    const __m256i X63 = _mm256_loadu_si256((__m256i *) (x + 252));
    const __m256i m0 = X0 + X32, m1 = X1 + X33;
    const __m256i M0 = X0 - X32, M1 = X1 - X33;
    i64_fwht128_terminale_avx2(m0, m1, X2 + X34, X3 + X35, X4 + X36, X5 + X37, X6 + X38, X7 + X39,
                               X8 + X40, X9 + X41, X10 + X42, X11 + X43, X12 + X44, X13 + X45, X14 + X46, X15 + X47,
                               X16 + X48, X17 + X49, X18 + X50, X19 + X51, X20 + X52, X21 + X53, X22 + X54, X23 + X55,
                               X24 + X56, X25 + X57, X26 + X58, X27 + X59, X28 + X60, X29 + X61, X30 + X62, X31 + X63, y + 0);
    i64_fwht128_terminale_avx2(M0, M1, X2 - X34, X3 - X35, X4 - X36, X5 - X37, X6 - X38, X7 - X39,
                               X8 - X40, X9 - X41, X10 - X42, X11 - X43, X12 - X44, X13 - X45, X14 - X46, X15 - X47,
                               X16 - X48, X17 - X49, X18 - X50, X19 - X51, X20 - X52, X21 - X53, X22 - X54, X23 - X55,
                               X24 - X56, X25 - X57, X26 - X58, X27 - X59, X28 - X60, X29 - X61, X30 - X62, X31 - X63, y + 128);
}

inline __m256i i64_fwht4_avx2(const __m256i X) {

    const __m256i neg34 = _mm256_sub_epi64(_mm256_setzero_si256(), X);
    const __m256i N0    = _mm256_blend_epi32(X, neg34, 0xF0);
    const __m256i N1    = _mm256_permute2x128_si256(X, X, 0x01);
    // const __m256i N1 = _mm256_permute4x64_epi64(X, 0x4E);
    const __m256i b     = _mm256_add_epi64(N0, N1);
    const __m256i neg13 = _mm256_sub_epi64(_mm256_setzero_si256(), b);
    const __m256i N2    = _mm256_blend_epi32(b, neg13, 0xCC);
    const __m256i N3    = _mm256_permute4x64_epi64(b, 0xB1);
    __m256i       Y     = _mm256_add_epi64(N2, N3);
    return (Y);
}

inline void i64_fwht8_avx2(__m256i * B0, __m256i * B1) {
    const __m256i C0 = _mm256_add_epi64(*B0, *B1);
    const __m256i C1 = _mm256_sub_epi64(*B0, *B1);
    *B0              = i64_fwht4_avx2(C0);
    *B1              = i64_fwht4_avx2(C1);
}

inline void fwht16_avx2(__m256i * B0, __m256i * B1, __m256i * B2, __m256i * B3) {
    __m256i C0 = _mm256_add_epi64(*B0, *B2);
    __m256i C1 = _mm256_add_epi64(*B1, *B3);

    __m256i C2 = _mm256_sub_epi64(*B0, *B2);
    __m256i C3 = _mm256_sub_epi64(*B1, *B3);

    i64_fwht8_avx2(&C0, &C1);
    i64_fwht8_avx2(&C2, &C3);

    *B0 = C0;
    *B1 = C1;
    *B2 = C2;
    *B3 = C3;
}

template <>
inline void i64_fwht_avx2<4>(int64_t x[]) {
    const __m256i C0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i D0 = i64_fwht4_avx2(C0);
    _mm256_storeu_si256((__m256i *) (x + 0), D0);
}
template <>
inline void i64_fwht_avx2<4>(int64_t x[], int64_t y[]) {
    const __m256i C0 = _mm256_loadu_si256((__m256i *) (x + 0));
    const __m256i D0 = i64_fwht4_avx2(C0);
    _mm256_storeu_si256((__m256i *) (y + 0), D0);
}

template <>
inline void i64_fwht_avx2<8>(int64_t x[]) {
    i64_fwht8_flat_avx2(x, x);
}
template <>
inline void i64_fwht_avx2<8>(int64_t x[], int64_t y[]) {
    i64_fwht8_flat_avx2(x, y);
}

template <>
inline void i64_fwht_avx2<16>(int64_t x[]) {
    i64_fwht16_flat_avx2(x, x);
}
template <>
inline void i64_fwht_avx2<16>(int64_t x[], int64_t y[]) {
    i64_fwht16_flat_avx2(x, y);
}

template <>
inline void i64_fwht_avx2<32>(int64_t x[]) {
    i64_fwht32_flat_avx2(x, x);
}
template <>
inline void i64_fwht_avx2<32>(int64_t x[], int64_t y[]) {
    i64_fwht32_flat_avx2(x, y);
}

template <>
inline void i64_fwht_avx2<64>(int64_t x[]) {
    i64_fwht64_flat_avx2(x, x);
}
template <>
inline void i64_fwht_avx2<64>(int64_t x[], int64_t y[]) {
    i64_fwht64_flat_avx2(x, y);
}

template <>
inline void i64_fwht_avx2<128>(int64_t x[]) {
    i64_fwht128_flat_avx2(x, x);
}
template <>
inline void i64_fwht_avx2<128>(int64_t x[], int64_t y[]) {
    i64_fwht128_flat_avx2(x, y);
}

template <>
inline void i64_fwht_avx2<256>(int64_t x[]) {
    i64_fwht256_flat_avx2(x, x);
}
template <>
inline void i64_fwht_avx2<256>(int64_t x[], int64_t y[]) {
    i64_fwht256_flat_avx2(x, y);
}

template <>
inline void i64_fwht_avx2<512>(int64_t x[]) {
    const int simd = sizeof(__m256i) / sizeof(int64_t);
    for (int i = 0; i < 256; i += simd) {
        const __m256i A = _mm256_loadu_si256((__m256i *) (x + i + 0));
        const __m256i B = _mm256_loadu_si256((__m256i *) (x + i + 256));
        const __m256i C = _mm256_add_epi64(A, B);
        const __m256i D = _mm256_sub_epi64(A, B);
        _mm256_storeu_si256((__m256i *) (x + i + 0), C);
        _mm256_storeu_si256((__m256i *) (x + i + 256), D);
    }
    i64_fwht256_flat_avx2(x + 0, x + 0);
    i64_fwht256_flat_avx2(x + 256, x + 256);
}

template <>
inline void i64_fwht_avx2<1024>(int64_t x[]) {
    const int simd = sizeof(__m256i) / sizeof(int64_t);
    for (int i = 0; i < 512; i += simd) {
        const __m256i A = _mm256_loadu_si256((__m256i *) (x + i + 0));
        const __m256i B = _mm256_loadu_si256((__m256i *) (x + i + 512));
        const __m256i C = _mm256_add_epi64(A, B);
        const __m256i D = _mm256_sub_epi64(A, B);
        _mm256_storeu_si256((__m256i *) (x + i + 0), C);
        _mm256_storeu_si256((__m256i *) (x + i + 512), D);
    }
    i64_fwht_avx2<512>(x + 0);
    i64_fwht_avx2<512>(x + 512);
}

// template <>
// inline void i64_fwht_avx2<8>(int64_t x[]) {
//     const __m256i P0 = _mm256_loadu_si256((__m256i *)(x + 0));
//     const __m256i N0 = _mm256_loadu_si256((__m256i *)(x + 4));
//     const __m256i S0 = _mm256_add_epi64(P0, N0);
//     const __m256i S1 = _mm256_sub_epi64(P0, N0);

//     const __m256i D0 = i64_fwht4_terminale_avx2(S0);
//     const __m256i D1 = i64_fwht4_terminale_avx2(S1);
//     _mm256_storeu_si256((__m256i *)(x + 0), D0);
//     _mm256_storeu_si256((__m256i *)(x + 4), D1);

// }

// template <>
// inline void i64_fwht_avx2<16>(int64_t x[]) {
//     const __m256i P0 = _mm256_loadu_si256((__m256i *)(x + 0));
//     const __m256i P1 = _mm256_loadu_si256((__m256i *)(x + 4));
//     const __m256i N0 = _mm256_loadu_si256((__m256i *)(x + 8));
//     const __m256i N1 = _mm256_loadu_si256((__m256i *)(x + 12));
//     const __m256i S0 = _mm256_add_epi64(P0, N0);
//     const __m256i S1 = _mm256_sub_epi64(P0, N0);

//     const __m256i D0 = i64_fwht4_terminale_avx2(S0);
//     const __m256i D1 = i64_fwht4_terminale_avx2(S1);
//     _mm256_storeu_si256((__m256i *)(x + 0), D0);
//     _mm256_storeu_si256((__m256i *)(x + 4), D1);

// }

#endif
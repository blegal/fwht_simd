//
// Created by legal on 03/07/2025.
//
#pragma once
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
#warning "Basic c code was included (normalize_c.hpp)
#include <cstdint>


//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
template <int gf_size>
void normalize(float * tab) {
    float sum = 1e-32f;
    for (int i = 0; i < gf_size; i += 1) {
        sum += tab[i];
    }
    const float factor = 1.f / sum;
    for (int i = 0; i < gf_size; i++) {
        tab[i] *= factor;
    }
}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//

//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
template <int gf_size>
void normalize(int32_t * tab) {
    uint32_t sum = 0;
    for (int i = 0; i < gf_size; i += 1) {
        sum |= tab[i];
    }

    //const int nZeros = countLeadingZeros(sum);

    int n;
    if (sum == 0)
        n = 32;
    else if ((sum >> 16) == 0) {
        n += 16;
        sum <<= 16;
    }
    if ((sum >> 24) == 0) {
        n += 8;
        sum <<= 8;
    }
    if ((sum >> 28) == 0) {
        n += 4;
        sum <<= 4;
    }
    if ((sum >> 30) == 0) {
        n += 2;
        sum <<= 2;
    }
    if ((sum >> 31) == 0) {
        n += 1;
    }
    const int nShift = n - 2; // on conserve 2 bits

    for (int i = 0; i < gf_size; i++) {
        tab[i] = tab[i] << nShift;
    }
}

template void normalize<64>(float * tab);
template void normalize<64>(int32_t * tab);
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//

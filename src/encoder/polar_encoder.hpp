#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include "encoder_1.hpp"

class polar_encoder {
public:
    polar_encoder(const uint16_t* vec, const int k, const int n) : K(k), N(n)
    {
        liste.resize( n );
        for (int i = 0; i < k; i++)
            liste[ i ] = vec[i];
    }

    void encode(uint16_t* dst, const uint16_t* src) const {

        for (int i = 0; i < K; i++)
            dst[ reliab_seq[i] ] = src[i];

        for (int i = K; i < N; i++)
            dst[ reliab_seq[i] ] = 0;

        polar_encode<64>(dst);
    }

    void decode(uint16_t* dst, const uint16_t* src) const {
        for (int i = 0; i < K; i++)
            dst[i] = src[ reliab_seq[i] ];
    }

private:
    const int N;
    const int K;
    std::vector<uint8_t> liste;
};

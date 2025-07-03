#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include "definitions/custom_types.hpp"

template<int GF>
class demodulator {
public:
    demodulator(const int n) : N(n)
    {
    }

    void demodulate(symbols_t* dst, const uint16_t* src) const {
        for (int i = 0; i < N; i++)
        {
            dst[i].is_freq = false;
            for (int j = 0; j < K; j++)
                dst[i].value[j] = 0.f;
            dst[i].value[ src[i] ] = 1.f;
        }
    }

private:
    const int N;
};

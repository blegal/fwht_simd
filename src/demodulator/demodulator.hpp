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
            for (int j = 0; j < GF; j++)    // toutes les probabilités à zero
                dst[i].value[j] = 0.f;      // pour l'initialisation
            dst[i].value[ src[i] ] = 1.f;   // on met le bon symbole à 100%
        }
    }

private:
    const int N;
};

#pragma once
#include <cstdio>

#include "definitions/const_config_GF64_N64.hpp"

struct symbols_t {
    float value[GF];
    //    uint8_t  gf   [GF]; // to be removed !
    bool is_freq;
};

template <int gf_size>
void normalize(float * tab) {
    float sum = 0.0000001f;
    for (int i = 0; i < gf_size; i += 1) {
        sum += tab[i];
    }
    const float factor = 1.f / sum;
    for (int i = 0; i < gf_size; i++) {
        tab[i] *= factor;
    }
}

template <int gf_size>
int argmax(const float * value) {
    int   max_index = 0;
    float max_value = value[0];
    for (int i = 1; i < gf_size; i++) {
        if (value[i] > max_value) {
            max_value = value[i];
            max_index = i;
        }
    }
    return max_index;
}

void show_symbols(const symbols_t * symbols);

void show_symbols(const float * symbols);


#pragma once
#include <cstdint>
#include "const_config_GF64_N64.hpp"
//
//
//
//
//
struct symbols_t
{
    float    value[GF];
    uint16_t gf   [GF]; // to be removed !
    bool is_freq;
};
//
//
//
//
//
template <int gf_size>
void normalize(float* tab)
{
    float sum = 0;
    for(int i = 0; i < gf_size; i += 1){
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
//
//
template <int gf_size>
int argmax(const float* value)
{
    int max_index = 0;
    float max_value = value[0];
    for (int i = 1; i < gf_size; i++) {
        if (value[i] > max_value) {
            max_value = value[i];
            max_index = i;
        }
    }
    return max_index;
}
//
//
//
//
//
void show_symbols(const symbols_t* symbols)
{
    if(symbols->is_freq )
        printf("Symbols [freq]:");
    else
        printf("Symbols [prob]:");

    float sum = 0.f;
    for (int i = 0; i < GF; i += 1)
    {
        if ( (i%16) == 0 ) printf("\n %2d : ", i);
        if( symbols->value[i] >= 0.2f )
            printf("\e[1;32m%+6.4f\e[0m ", symbols->value[i]);
        else
            printf("%+6.4f ", symbols->value[i]);
        sum += symbols->value[i];
    }
    printf("\n    : sum = %f\n", sum);
}
//
//
//
//
//

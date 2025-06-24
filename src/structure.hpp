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
void show_symbols(const symbols_t* symbols)
{
    if(symbols->is_freq )
        printf("Symbols [freq]:");
    else
        printf("Symbols [prob]:");
    for (int i = 0; i < GF; i += 1)
    {
        if ( (i%16) == 0 ) printf("\n %2d : ", i);
        printf("%+6.1f ", symbols->value[i]);
    }
    printf("\n");
}
//
//
//
//
//

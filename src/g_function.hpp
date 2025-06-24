#pragma once
//
//
//
//
//
#include "structure.hpp"
#include "fwht_x86.hpp"
#include "hadamard_64.hpp"
#include "const_config_GF64_N64.hpp"
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
void g_function(
    symbols_t* dst,     // the data to be computed for the left side of the graph
    symbols_t* src_a,   // the upper value set from the right side of the graph
    symbols_t* src_b,   // the lower value set from the right side of the graph
    uint16_t* src_c)    // the computed symbols coming from the left side of the graph
{
    symbols_t result;
    
    //
    // Currectly, we assume src_a is frequency domain
    //
    if( src_a->is_freq == false ) {
        fwht<gf_size>( src_a->value );
        src_a->is_freq = true;
    }

    //
    //  Process the mulitplication to support precomputed symbols
    //
    int index = src_c[0];
    const float* toto/*[64]*/ = Hadamard[ index ]; // ligne de la matrice de Hadamard
    for (size_t i = 0; i < gf_size; i++)
        src_a->value[i] = src_a->value[i] * toto[i];

    if( src_a->is_freq == true ) {
        fwht<gf_size>( src_a->value );
        src_a->is_freq = false;
    }

    if( src_b->is_freq == true ) {
        fwht<gf_size>( src_b->value );
        src_b->is_freq = false;
    }

    // Abdallah computations ...
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
        dst->gf   [i] = src_a->gf   [i]; // to be removed !
    }
    normalize<gf_size>( dst->value );
    // Abdallah computations ...    
}
//
//
//
//
//

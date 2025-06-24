#pragma once
//
//
//
//
//
#include "../structure.hpp"
#include "../fwht/fwht_x86.hpp"
#include "../hadamard/hadamard_64.hpp"
#include "../const_config_GF64_N64.hpp"
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
    uint32_t   src_c)   // the computed symbols coming from the left side of the graph
{
    symbols_t result;
    
    //
    // Currectly, we assume src_a is frequency domain
    //
//    if( src_a->is_freq == false ) {
//        fwht<gf_size>( src_a->value );
//        src_a->is_freq = true;
//    }
//    int index = src_c[0];
//    const float* toto/*[64]*/ = Hadamard[ index ]; // ligne de la matrice de Hadamard
//    for (size_t i = 0; i < gf_size; i++)
//        src_a->value[i] = src_a->value[i] * toto[i];
    //
    //  Process the mulitplication to support precomputed symbols
    //

    if( src_a->is_freq == true )
    {
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
        const int idx = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
        dst->gf   [i  ] = src_a->gf   [i]; // to be removed !
    }
    normalize<gf_size>( dst->value );
    src_b->is_freq = false;
}
//
//
//
//
//

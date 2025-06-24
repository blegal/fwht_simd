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
    //
    //  Process the mulitplication to support precomputed symbols
    //

    if( src_a->is_freq == true )
    {
        fwht<gf_size>( src_a->value );
        normalize<gf_size>(src_a->value, 0.125);
        src_a->is_freq = false;
    }

    if( src_b->is_freq == true ) {
        fwht<gf_size>( src_b->value );
        normalize<gf_size>(src_b->value, 0.125);
        src_b->is_freq = false;
    }

    // Abdallah computations ...
    for (size_t i = 0; i < gf_size; i++)
    {
        const int idx = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
        dst->gf   [i  ] = src_a->gf   [i]; // to be removed !
    }

//    printf("##################################################\n");
//    show_symbols( src_a );
//    show_symbols( src_b );
//    show_symbols( dst   );

    normalize<gf_size>( dst->value ); // temporal
    src_b->is_freq = false;
}
//
//
//
//
//

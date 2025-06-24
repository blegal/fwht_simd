
#include "../src/fwht/fwht_x86.hpp"
#include "../src/fwht/fwht_neon.hpp"
#include "../src/fwht/fwht_avx2.hpp"
#include <cstring>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "../src/const_config_GF64_N64.hpp"
#include "../src/hadamard/hadamard_64.hpp"
#include "../src/structure.hpp"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

#include "../src/nodes/top_node.hpp"
//
//
//
// In frozen symbol array, the value -1 means the symbol is frozen => (symbol = 0)
//
int frozen_symbols[64];

int main(int argc, char* argv[])
{
    const int size = 64;

    //
    // load the channel symbols
    //
    symbols_t* channel  = new symbols_t[N];
    for (int i = 0; i < N; i += 1)
    {
        for (int j = 0; j < GF; j += 1)
        {
            channel[i].value[j] = chan[GF * i + j];
            channel[i].gf   [j] = j;
        }
//      normalize< 64>( channel[i].value ); // added probability normalization BLG
    }

    //
    // Clear the internal memory space
    //
    symbols_t* internal = new symbols_t[N];
    for (int i = 0; i < N; i += 1)
    {
        for (int j = 0; j < GF; j += 1)
        {
            internal[i].value[j] = 0.f;
            internal[i].gf   [j] = j;
        }
    }

    //
    // initialize the frozen symbols array
    //
    uint16_t*  symbols  = new uint16_t[N];
    for (int i = 0; i < N; i += 1)
        frozen_symbols[i] = -1;
    for (int i = 0; i < K; i += 1)
        frozen_symbols[ reliab_seq[i] ] = i; // i c'est pour le DEBUG, on pourrait mettre 0
    
    //
    // Clear the decoded symbols array
    //
    uint16_t*  decoded  = new uint16_t [size];
    for (int i = 0; i < N; i += 1)
        decoded[i] = 0;

    //
    // Call the top node function to decode the symbols
    //
    top_node<64>(channel, internal, decoded, symbols, size);



    printf("\nFrozen matrix:\n");
    for (int i = 0; i < N; i += 1)
    {
        if( (i%16) == 0 )
            printf("\n ");
        printf("%2d ", frozen_symbols[i]);
    }printf("\n");

    printf("\n\nDecoded symbols:\n");
    for (int i = 0; i < N; i += 1)
    {
        if( (i%16) == 0 )
            printf("\n ");
        if (decoded[i] == ref_out[i]){
             printf("\e[1;32m%2d\e[0m ", decoded[i]);
        }else{
             printf("\e[1;31m%2d\e[0m ", decoded[i]);
        }
        
       
    }printf("\n");

    delete[] channel;
    delete[] internal;
    delete[] symbols;
    delete[] decoded;
    
    return EXIT_SUCCESS;
}
    
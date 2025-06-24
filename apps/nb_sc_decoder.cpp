
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
        symbols[i] = -1;
    for (int i = 0; i < K; i += 1)
        symbols[ reliab_seq[i] ] = 0;
    
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

    delete[] channel;
    delete[] internal;
    delete[] symbols;
    delete[] decoded;
    
    return EXIT_SUCCESS;
}
    

#include "../src/fwht/fwht_x86.hpp"
#include "../src/fwht/fwht_neon.hpp"
#include "../src/fwht/fwht_avx2.hpp"
#include <cstring>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <chrono>

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
    // Load the channel symbols
    //
    symbols_t* channel  = new symbols_t[N];
    for (int i = 0; i < N; i += 1)
    {
        for (int j = 0; j < GF; j += 1)
        {
            channel[i].value[j] = chan[GF * i + j];
//            channel[i].gf   [j] = j;
            channel[i].is_freq  = false;
        }
        normalize<GF>( channel[i].value );
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
//            internal[i].gf   [j] = j;
        }
    }

    printf("#\n");
    printf("###########################################################\n");
    printf("#\n");

    for (int i = 0; i < 4; i += 1)
        show_symbols( channel + i );

    printf("#\n");
    printf("###########################################################\n");
    printf("#\n");

    for (int i = 0; i < 2; i += 1)
    {
        f_function<64>( internal + i, channel + i, channel + 2 + i );
    }

    for (int i = 0; i < 2; i += 1)
        show_symbols( internal + i );

    printf("#\n");
    printf("###########################################################\n");
    printf("#\n");

    f_function<64>( internal + 2, internal + 0, internal + 1 );
    show_symbols( internal + 2 );

    //
    //
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //
    //

    printf("#\n");
    printf("###########################################################\n");
    printf("#\n");

    for (int i = 0; i < 2; i += 1)
    {
        f_function<64>( internal + i, channel + i, channel + 2 + i );
    }

    for (int i = 0; i < 2; i += 1)
        show_symbols( internal + i );

    printf("#\n");
    printf("###########################################################\n");
    printf("#\n");

    f_function<64>( internal + 2, internal + 0, internal + 1 );
    show_symbols( internal + 2 );

    f_function<64>( internal + 2, internal + 0, internal + 1 );
    show_symbols( internal + 2 );

    
    delete[] channel;
    delete[] internal;

    return EXIT_SUCCESS;
}
    
 // 0 : +0.0114 +0.0034 +0.0141 +0.0090 +0.0202 +0.0069 +0.0663 +0.0182 +0.0061 +0.0076 +0.0152 +0.0059 +0.0604 +0.0063 +0.0080 +0.0066 
 // 16 : +0.0574 +0.0082 +0.0072 +0.0099 +0.0126 +0.0083 +0.0087 +0.0091 +0.0229 +0.0075 +0.0594 +0.0218 +0.0078 +0.0057 +0.0156 +0.0081 
 // 32 : +0.0057 +0.0101 +0.0196 +0.0118 +0.0114 +0.0075 +0.0143 +0.0066 +0.0036 +0.0086 +0.0391 +0.0075 +0.0074 +0.0410 +0.0179 +0.0081 
 // 48 : +0.0077 +0.0462 +0.0196 +0.0085 +0.0053 +0.0050 +0.0448 +0.0069 +0.0125 +0.0053 +0.0213 +0.0061 +0.0071 +0.0141 +0.0251 +0.0090 

 //  0 : +0.0114 +0.0034 +0.0141 +0.0090 +0.0202 +0.0069 +0.0663 +0.0182 +0.0061 +0.0076 +0.0152 +0.0059 +0.0604 +0.0063 +0.0080 +0.0066 
 // 16 : +0.0574 +0.0082 +0.0072 +0.0099 +0.0126 +0.0083 +0.0087 +0.0091 +0.0229 +0.0075 +0.0594 +0.0218 +0.0078 +0.0057 +0.0156 +0.0081 
 // 32 : +0.0057 +0.0101 +0.0196 +0.0118 +0.0114 +0.0075 +0.0143 +0.0066 +0.0036 +0.0086 +0.0391 +0.0075 +0.0074 +0.0410 +0.0179 +0.0081 
 // 48 : +0.0077 +0.0462 +0.0196 +0.0085 +0.0053 +0.0050 +0.0448 +0.0069 +0.0125 +0.0053 +0.0213 +0.0061 +0.0071 +0.0141 +0.0251 +0.0090 

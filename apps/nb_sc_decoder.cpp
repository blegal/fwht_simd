
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
#include "../src/nodes/top_node_with_pruning.hpp"

#include "../src/pruning/top_node_v2.hpp"

//
//
//
// In frozen symbol array, the value -1 means the symbol is frozen => (symbol = 0)
//
int frozen_symbols[64];

enum next_node { RATE_0,
                RATE_1_FROM_F,
                RATE_1_FROM_G,
                LEAF_RATE_0,
                LEAF_RATE_1_FROM_F,
                LEAF_RATE_1_FROM_G,
                MID_NODE_FROM_F,
                MID_NODE_FROM_G};
next_node pruning[2 * N];

int frozen_tree(
    int* frozen,
    int curr_frozen,
    next_node* array,
    int curr_elmnt,
    const int size,
    const int level = 1)
{
    const int n = size / 2; // Assuming size is the number of symbols
    for(int z = 0; z < level; z += 1) printf("+  "); printf("NODE LEVEL (%d)\n", size); 
    for(int z = 0; z < level; z += 1) printf("+  ") ; 
    printf("F edge : ") ; 
    for(int i = 0; i < n; i++){
        if( i == n ) printf(" | ");
        printf("%d", frozen_symbols[curr_frozen + i]);
    }
    printf("\n");

    //
    // Analyse de la branche gauche
    //
    int sum = 0;
    for(int i = 0; i < n; i++)
        sum += frozen_symbols[curr_frozen + i];

    int next_elmnt;
    if( sum == n ) {
        if( n == 1 ){
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Leaf rate-0 node found (%d)\n", n);
            array[curr_elmnt] = LEAF_RATE_0;
        }else{
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Rate-0 node found (%d)\n", n);
            array[curr_elmnt] = RATE_0;
        }
        next_elmnt = curr_elmnt + 1;
    } else if( sum == 0 ) {
        if( n == 1 ) {
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Leaf rate-1 node found (%d)\n", n);
            array[curr_elmnt] = LEAF_RATE_1_FROM_F;
        }else{
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Rate-1 node found (%d)\n", n);
            array[curr_elmnt] = RATE_1_FROM_F;
        }
        next_elmnt = curr_elmnt + 1;
    }else{
        for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Normal (f) node found (%d)\n", n);
        array[curr_elmnt] = MID_NODE_FROM_F;
        next_elmnt = frozen_tree(
            frozen,
            curr_frozen,
            array,
            curr_elmnt + 1,
            n, level + 1);
    }

    //
    // Analyse de la branche droite
    //
    for(int z = 0; z < level; z += 1) printf("+  ") ; 
    printf("G edge : ") ; 
    for(int i = 0; i < n; i++)
        printf("%d", frozen_symbols[curr_frozen + n + i]);
    printf("\n");

    sum = 0;
    for(int i = 0; i < n; i++)
        sum += frozen_symbols[curr_frozen + n + i];
    if( sum == n ) {
        if( n == 1 ){
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Leaf rate-0 node found (size = %d)\n", n);
            array[curr_elmnt] = LEAF_RATE_0;
        }else{
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Rate-0 node found (size = %d)\n", n);
            array[curr_elmnt] = RATE_0;
        }
        return curr_elmnt + 1;
    }else  if( sum == 0 ) {
        if( n == 1 ) {
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Leaf rate-1 node found (size = %d)\n", n);
            array[curr_elmnt] = LEAF_RATE_1_FROM_G;
        }else{
            for(int z = 0; z < level; z += 1) printf("+  ") ; printf("> Rate-1 node found (size = %d)\n", n);
            array[curr_elmnt] = RATE_1_FROM_G;
        }
        return curr_elmnt + 1;
    }else{
        for(int z = 0; z < level; z += 1) printf("+  ") ; printf(" >Normal (g) node found (size = %d)\n", n);
        array[next_elmnt] = MID_NODE_FROM_G;
        int final_elmnt = frozen_tree(
            frozen,
            curr_frozen + n,
            array,
            next_elmnt + 1,
            n, level + 1);
        return final_elmnt;
    }
}


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
//            channel[i].gf   [j] = j;
            channel[i].is_freq  = false;
        }
//      normalize< 64>( channel[i].value ); // added probability normalization BLG
        normalize<GF>( channel[i].value );
//      show_symbols( channel + i );
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

    //
    // initialize the frozen symbols array
    //
    uint16_t*  symbols  = new uint16_t[N];
    for (int i = 0; i < N; i += 1)
        frozen_symbols[i] = true;
    for (int i = 0; i < K; i += 1)
        frozen_symbols[ reliab_seq[i] ] = false; // i c'est pour le DEBUG, on pourrait mettre 0

    const int n_nodes = frozen_tree(frozen_symbols, 0, pruning, 0, N);
    printf("\nFrozen matrix:\n");
    for (int i = 0; i < N; i += 1)
    {
        if( (i%8 ) == 0 ) printf(" | ");
        if( (i%16) == 0 ) printf("\n | ");
        printf("%2d ", frozen_symbols[i]);
    }printf(" |\n");

    //
    // Clear the decoded symbols array
    //
    uint16_t*  decoded  = new uint16_t [size];
    for (int i = 0; i < N; i += 1)
        decoded[i] = 0;

    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    //
    // Call the top node function to decode the symbols
    //
    top_node<64>(channel, internal, decoded, symbols, size);
    printf("\n\nDecoded symbols (normal):\n");
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


    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    //
    // Call the top node function to decode the symbols
    //
    top_node_with_pruning<64>(channel, internal, decoded, symbols, size);
    printf("\n\nDecoded symbols (pruning):\n");
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

    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    //
    // Call the top node function to decode the symbols
    //
    nb_decoder_v2<64> dec;
    dec.execute(channel, internal, decoded, symbols, size);
    printf("\n\nDecoded symbols (class):\n");
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

    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    //
    //

    const  int32_t nTest = (256 * 1024);

    auto start_x86 = std::chrono::system_clock::now();
    for(int32_t loop = 0; loop < nTest; loop += 1)
    {
        top_node<64>(channel, internal, decoded, symbols, size);
    }
    auto stop_x86 = std::chrono::system_clock::now();

    float time_ns   = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count();
    float time_sec  = time_ns / 1000.f / 1000.f / 1000.f; // in seconds
    float time_msec = time_ns / 1000.f / 1000.f; // in seconds
    float time_usec = time_ns / 1000.f; // in seconds
    float time_run  = (time_usec / (float)nTest);

    float debit = ((float)N * (float)logGF) / time_run; // in Ksymbols/s
    printf("[normal]  experiments  : %1.3f sec\n",  time_sec);
    printf("[normal]  experiments  : %1.2f ms\n",   time_msec);
    printf("[normal]  one decoding : %1.2f us\n",   time_run);
    printf("[normal]  debit coded  : %1.2f Mbps\n", debit);
    //
    //
    /////////////////////////////////////////////////////////////////////////////////
    //
    //
    start_x86 = std::chrono::system_clock::now();
    for(int32_t loop = 0; loop < nTest; loop += 1)
    {
        top_node_with_pruning<64>(channel, internal, decoded, symbols, size);
    }
    stop_x86 = std::chrono::system_clock::now();

    time_ns   = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count();
    time_sec  = time_ns / 1000.f / 1000.f / 1000.f; // in seconds
    time_msec = time_ns / 1000.f / 1000.f; // in seconds
    time_usec = time_ns / 1000.f; // in seconds
    time_run  = (time_usec / (float)nTest);

    debit = ((float)N * (float)logGF) / time_run; // in Ksymbols/s
    printf("[pruning] experiments  : %1.3f sec\n",  time_sec);
    printf("[pruning] experiments  : %1.2f ms\n",   time_msec);
    printf("[pruning] one decoding : %1.2f us\n",   time_run);
    printf("[pruning] debit coded  : %1.2f Mbps\n", debit);
    //
    //
    /////////////////////////////////////////////////////////////////////////////////
    //
    //
    start_x86 = std::chrono::system_clock::now();
    for(int32_t loop = 0; loop < nTest; loop += 1)
    {
        dec.execute(channel, internal, decoded, symbols, size);
    }
    stop_x86 = std::chrono::system_clock::now();

    time_ns   = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count();
    time_sec  = time_ns / 1000.f / 1000.f / 1000.f; // in seconds
    time_msec = time_ns / 1000.f / 1000.f; // in seconds
    time_usec = time_ns / 1000.f; // in seconds
    time_run  = (time_usec / (float)nTest);

    debit = ((float)N * (float)logGF) / time_run; // in Ksymbols/s
    printf("[special] experiments  : %1.3f sec\n",  time_sec);
    printf("[special] experiments  : %1.2f ms\n",   time_msec);
    printf("[special] one decoding : %1.2f us\n",   time_run);
    printf("[special] debit coded  : %1.2f Mbps\n", debit);
    //
    //
    /////////////////////////////////////////////////////////////////////////////////
    //
    //
    while( true )
        dec.execute(channel, internal, decoded, symbols, size);

    delete[] channel;
    delete[] internal;
    delete[] symbols;
    delete[] decoded;
    
    return EXIT_SUCCESS;
}
    
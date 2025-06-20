/*
*	Optimized bit-packing and bit-unpacking functions - Copyright (c) 2021 Bertrand LE GAL
*
*  This software is provided 'as-is', without any express or
*  implied warranty. In no event will the authors be held
*  liable for any damages arising from the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute
*  it freely, subject to the following restrictions:
*
*  1. The origin of this software must not be misrepresented;
*  you must not claim that you wrote the original software.
*  If you use this software in a product, an acknowledgment
*  in the product documentation would be appreciated but
*  is not required.
*
*  2. Altered source versions must be plainly marked as such,
*  and must not be misrepresented as being the original software.
*
*  3. This notice may not be removed or altered from any
*  source distribution.
*
*/
#include "../src/fwht_x86.hxx"
#include "../src/fwht_neon.hxx"
#include "../src/fwht_avx2.hxx"
#include <cstring>
#include <chrono>

bool are_equivalent(float* a, float* b, float epsilon, int size) {
    for (int i = 0; i < size; i++) {
        float diff = abs(a[i] - b[i]);
        if (diff > epsilon) {
            printf("- maximum absolute error is : %f\n", diff);
            printf("- a[%d] = %f and b[%d] = %f\n", i, a[i], i, b[i]);
            return false;
        }
    }
    return true;
}
//
//
//
//
//
struct symbols_t
{
    float llr[64];
    char  gf [64];
    bool  is_freq;
};
//
//
//
//
//
template <int gf_size>
symbols_t f_function(symbols_t* dst, const symbols_t* src_a, const symbols_t* src_b)
{
    //
    // Switch from time to frequency domain if needed
    //
    if( src_a->is_freq == false ) {
        fwht<gf_size>( src_a->llr );
        src_a->is_freq = true;
    }

    if( src_b->is_freq == false ) {
        fwht<gf_size>( src_b->llr );
        src_b->is_freq = true;
    }

    // Abdallah computations ...
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->llr[i] = src_a->llr[i] * src_b->llr[i];
        dst->gf[i]  = src_a->gf [i] ^ src_b->gf [i];
    }
    // Abdallah computations ...    
}
//
//
//
//
//
template <int gf_size>
symbols_t g_function(symbols_t* dst, const symbols_t* src_a, const symbols_t* src_b)
{
    symbols_t result;
    //
    // Switch from time to frequency domain if needed
    //
    if( src_a->is_freq == true ) {
        fwht<gf_size>( src_a->llr );
        src_a->is_freq = false;
    }

    if( src_b->is_freq == true ) {
        fwht<gf_size>( src_b->llr );
        src_b->is_freq = false;
    }

    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
}
//
//
//
//
//
template <int gf_size>
void final_node(symbols_t* var)
{
    //
    // Switch from frequency to time domain if needed
    //
    if( var->is_freq ) {
        fwht<gf_size>( var->llr );
        var->is_freq = false;
    }

    int max_index = 0;
    float max_value = var->llr[0];
    for (int i = 1; i < gf_size; i++) {
        if (var->llr[i] > max_value) {
            max_value = var->llr[i];
            max_index = i;
        }
    }

    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...

    return; // ???
}
//
//
//
//
//
template <int gf_size>
void middle_node(symbols_t* inputs, symbols_t* internal, int size)
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>( internal + i, inputs + i, inputs + n + i ); // Example operation
    }
    //
    // 
    //
    if( size == 1 ) {
        final_node<gf_size>(internal); // If we reach the final node, process it
    }else{
        middle_node<gf_size>(internal, internal + n, n/2); // On descend à gauche
    }
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>( internal, inputs + i, inputs + n + i); // Example operation
    }
    //
    // 
    //
    middle_node<gf_size>(internal, internal + n, n/2); // On descend à droite
    //
    // 
    //
    // should we do something here ?
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    //
    // 
    //
}
//
//
//
//
//
template <int gf_size = 64>
void top_node(symbols_t* channel, symbols_t* internal, const int size)
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>( internal + i, channel + i, channel + n + i ); // Example operation
    }
    //
    // 
    //
    middle_node<gf_size>(internal, internal + n, n/2); // On descend à gauche
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>( internal + i, channel + i, channel + n + i ); // Example operation
    }
    //
    // 
    //
    middle_node<gf_size>(internal, internal + n, n/2); // On descend à droite
    //
    // 
    //
    // should we do something here ?
    //
    // 
    //
}
//
//
//
//
//
int main(int argc, char* argv[])
{
    const int size = 64;

    symbols_t* channel  = new symbols_t[size];
    symbols_t* internal = new symbols_t[size];

    return EXIT_SUCCESS;
}
    

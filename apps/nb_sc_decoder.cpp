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
const int frozen_symbols[] = {-1,  1, -1,  2,  3, -1,  4,  5,
                               6, -1,  7,  8, -1,  9, 10, 11,
                              12, -1, 13, 14, -1, 15, 16, 17,
                              18, -1, 19, 20, -1, 21, 22, 23,
                              24, -1, 25, 26, -1, 27, 28, 29,
                              30, -1, 31, 32, -1, 33, 34, 35,
                              -1, 36, 37, 38, -1, -1, 39, 40,
                              -1, 41, 42, 43, -1, -1, 44, 45};
//
//
//
//
//
struct symbols_t
{
    float value[64];
    char  gf [64];
    bool  is_freq;
};
//
//
//
//
//
template <int gf_size>
void f_function(symbols_t* dst, symbols_t* src_a, symbols_t* src_b)
{
    //
    // Switch from time to frequency domain if needed
    //
    if( src_a->is_freq == false ) {
        fwht<gf_size>( src_a->value );
        src_a->is_freq = true;
    }

    if( src_b->is_freq == false ) {
        fwht<gf_size>( src_b->value );
        src_b->is_freq = true;
    }

    // Abdallah computations ...
    for (size_t i = 0; i < gf_size; i++)
    {
        dst->value[i] = src_a->value[i] * src_b->value[i];
        dst->gf   [i]  = src_a->gf  [i] ^ src_b->gf   [i];
    }
    // Abdallah computations ...    
}
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
void g_function(symbols_t* dst, symbols_t* src_a, symbols_t* src_b)
{
    symbols_t result;
    //
    // Switch from time to frequency domain if needed
    //
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
        dst->value[i] = src_a->value[i] + src_b->value[i];
        dst->gf   [i] = src_a->gf   [i];
    }
    normalize<gf_size>( dst->value );
    // Abdallah computations ...    
}
//
//
//
//
//
template <int gf_size>
int32_t final_node(symbols_t* var, int16_t* decoded, const int symbol_id)
{
    printf("-> final_node(%d) : frozen = %d\n", symbol_id, frozen_symbols[symbol_id]);
    if( frozen_symbols[symbol_id] == -1 )
    {
        return frozen_symbols[symbol_id];
    }
    //
    // Switch from frequency to time domain if needed
    //
    if( var->is_freq ) {
        fwht<gf_size>( var->value );
        var->is_freq = false;
    }

    int max_index = 0;
    float max_value = var->value[0];
    for (int i = 1; i < gf_size; i++) {
        if (var->value[i] > max_value) {
            max_value = var->value[i];
            max_index = i;
        }
    }

    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    // Abdallah computations ...
    decoded[symbol_id] = max_index;

    return max_index; // I suppose this is the index of the maximum value
}
//
//
//
//
//
template <int gf_size>
void middle_node(symbols_t* inputs, symbols_t* internal, int16_t* decoded, int size, const int symbol_id)
{
#if defined(__DEBUG__)
    printf("- middle_node(%d, %d)\n", size, symbol_id);
#endif
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
#if defined(__DEBUG__)
    printf("- f_function\n");
#endif
    for (int i = 0; i < n; i++) {
        f_function<gf_size>( internal + i, inputs + i, inputs + n + i ); // Example operation
    }
    //
    // 
    //
    if( n == 1 ) {
        final_node<gf_size>(internal, decoded, symbol_id); // If we reach the final node, process it
    }else{
        middle_node<gf_size>(internal, internal + n, decoded, n, symbol_id); // On descend à gauche
    }
    //
    // 
    //
#if defined(__DEBUG__)
    printf("- g_function\n");
#endif
    for (int i = 0; i < n; i++) {
        g_function<gf_size>( internal, inputs + i, inputs + n + i); // Example operation
    }
    //
    // 
    //
    if( n == 1 ) {
        final_node<gf_size>(internal, decoded, symbol_id + n); // If we reach the final node, process it
    }else{
        middle_node<gf_size>(internal, internal + n, decoded, n, symbol_id + n); // On descend à droite
    }
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
void top_node(symbols_t* channel, symbols_t* internal, int16_t* decoded, const int size)
{
#if defined(__DEBUG__)
    printf("top_node(%d)\n", size);
#endif
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
    middle_node<gf_size>(internal, internal + n, decoded, n, 0); // On descend à gauche
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>( internal + i, channel + i, channel + n + i ); // Example operation
    }
    //
    // 
    //
    middle_node<gf_size>(internal, internal + n, decoded, n, n); // On descend à droite
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
    int16_t*   decoded  = new int16_t  [size];
    top_node<64>(channel, internal, decoded, size);

    delete[] channel;
    delete[] internal;

    return EXIT_SUCCESS;
}
    
